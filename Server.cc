#include <iostream>
#include <pthread.h>

#include "netlink/netlink.h"
#include "message.pb.h"

using namespace std;
using namespace NL;

class ReceiveDeal : public SocketGroupCmd {
public:
    virtual void exec(Socket* socket, SocketGroup* group, void* reference) {
    	try {
    		char bufRecv[256];
    		memset(bufRecv, 0, 256);

    		int len = socket->read(bufRecv, 256);

        	cout << "\nreceived from Client: len = " << len << endl;

		   	Msg msgRecv;
		   	msgRecv.ParseFromString(bufRecv);

		   	msgRecv.PrintDebugString(); //debug print for check
		   	cout << "msgRecv.token = " << msgRecv.token() << endl;

        	socket->send(bufRecv, len);
        } catch (NL::Exception e) {
	        cout << "\nServer Received error: " << e.what() << endl;
	    } catch (exception e) {
	        cout << "\nServer Received other error: " << e.what() << endl;
	    }
    }
};

class DisconnectDeal : public SocketGroupCmd {
public:
    virtual void exec(Socket* socket, SocketGroup* group, void* reference) {
    	cout << "\nClient Disconnect: " << endl;

        group->remove(socket);
    }
};

void * work_thread(void * arg) {
	pthread_t pid = pthread_self();

	cout << "\nwork_thread:" << pid << " beginning." << endl;
	pthread_detach(pid);

	try {
		SocketGroup * pSG = new SocketGroup();
	    pSG->setCmdOnRead(new ReceiveDeal());
	    pSG->setCmdOnDisconnect(new DisconnectDeal());

	    Socket * pSK = (Socket *) arg; // 和客户端通信的socket
	    pSG->add(pSK);

		while (true) {
			pSG->listen(100);

			if (pSG->size() == 0) { // DisconnectDeal::exec 中 remove Socket 指针，size 就变为 0
				break;
			}
		}

		cout << "\nwork_thread:" << pid << " end." << endl;

		return NULL;
	} catch (NL::Exception e) {
        cout << "\nwork_thread socket error: " << e.what() << endl;
        return NULL;
    } catch (exception e) {
        cout << "\nwork_thread other error: " << e.what() << endl;
        return NULL;
    }
}

class AcceptDeal : public SocketGroupCmd {
public:
    virtual void exec(Socket* socket, SocketGroup* group, void* reference) {
        cout << "\nServer on Accept" << endl;
        Socket* pClientSK = socket->accept();

        // 创建一个线程和客户端通信
        pthread_t pid = 0;
        int ret = pthread_create(&pid, NULL, work_thread, pClientSK);
        if (ret != 0) {
            string msg("create listen_thread error: ");
            msg.append(strerror(ret));
            cout << "\nthread error msg: " << msg.c_str() << endl;
            throw NL::Exception(NL::Exception::ERROR_ALLOC, msg);
        }
    }
};

int main(int argc, char ** argv) {
	if (argc != 2) {
		cout << "usage: \n" << argv[0] << " 9527" << endl;
		exit(0);
	}

	try {
		init();

		Socket * pSK = new Socket(atoi(argv[1]), TCP, IP4, "localhost", 5);

		SocketGroup * pSG = new SocketGroup();
	    pSG->setCmdOnAccept(new AcceptDeal());
	    pSG->add(pSK);

	    while (true) {
	        try {
	            pSG->listen(100);
	        } catch (NL::Exception e) {
	            cout << "\nServer listen error: " << e.what() << endl;
				pSG->remove(pSK);
	        } catch (exception e) {
	            cout << "\nServer listen other error: " << e.what() << endl;
	        } catch (...) {
				cout << "\nServer unknown exception" << endl;
			}
		}

	} catch (NL::Exception e) {
        cout << "\nsocket init error: " << e.what() << endl;
        return -1;
    } catch (exception e) {
        cout << "\nconnect other error: " << e.what() << endl;
        return -1;
    }


	return 0;
}
