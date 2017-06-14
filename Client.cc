#include <iostream>

#include "netlink/netlink.h"
#include "message.pb.h"

using namespace std;
using namespace NL;

static void waitInput(Socket* socket) {
	static int token = 0;

	cout << "\nPlease enter somthing: " << endl;
   	char bufSend[256];
   	cin.getline(bufSend, 256); // 读一行，忽略空格和制表符

   	int len = strlen(bufSend);

   	if ( len > 0) {
   		if (strncmp(bufSend, "quit", len) == 0) {
   			cout << "Bye Bye!" << endl;
   			exit(0);
   		}

   		Msg msgSend;
	   	msgSend.set_len(strlen(bufSend));

	   	string strSend(bufSend);
	   	msgSend.set_content(strSend);
	   	msgSend.set_ok(true);
	   	msgSend.set_token(token);

	   	msgSend.PrintDebugString(); //debug print for check
	   	cout << "msgSend.token = " << msgSend.token() << endl;

	   	string strMsg;
	   	msgSend.SerializeToString(&strMsg);
   		socket->send(strMsg.c_str(), strMsg.size());
   	}
}

class ReceiveDeal : public SocketGroupCmd {
public:
    virtual void exec(Socket* socket, SocketGroup* group, void* reference) {
        char bufRecv[256];
		memset(bufRecv, 0, 256);

		int len = socket->read(bufRecv, 256);

    	cout << "\nreceived from Server: len = " << len << endl;

    	Msg msgRecv;
	   	msgRecv.ParseFromString(bufRecv);

	   	msgRecv.PrintDebugString(); //debug print for check

        waitInput(socket);
    }
};

class DisconnectDeal : public SocketGroupCmd {
public:
    virtual void exec(Socket* socket, SocketGroup* group, void* reference) {
        cout << "\nServer disconnect" << endl;
        group->remove(socket);
    }
};

int main(int argc, char ** argv) {
	if (argc != 2) {
		cout << "usage: \n" << argv[0] << " 9527" << endl;
		exit(0);
	}

	// Verify that the version of the library that we linked against is
  	// compatible with the version of the headers we compiled against.
  	GOOGLE_PROTOBUF_VERIFY_VERSION;

	try {
		init();

		SocketGroup * pSG = new SocketGroup();
	    pSG->setCmdOnRead(new ReceiveDeal());
	    pSG->setCmdOnDisconnect(new DisconnectDeal());

	    Socket * pSK = new Socket("localhost", atoi(argv[1]));
	    pSG->add(pSK);

	    waitInput(pSK);

	    while (true) {
	        try {
	      		pSG->listen(100);
	        } catch (NL::Exception e) {
	            cout << "\nClient listen error: " << e.what() << endl;
				pSG->remove(pSK);
	        } catch (exception e) {
	            cout << "\nClient listen other error: " << e.what() << endl;
	        } catch (...) {
				cout << "\nClient unknown exception" << endl;
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
