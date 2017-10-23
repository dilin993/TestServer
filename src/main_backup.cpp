//#include "MyTypes.h"
//#include "Server.h"
//
//const char *BINARY_MASK_R = "Binary Mask Received";
//
//int main()
//{
//    try
//    {
//        Server server(8080);
//        Mat binMask;
//        namedWindow(BINARY_MASK_R, CV_WINDOW_AUTOSIZE);
//
//        while(true)
//        {
//            try
//            {
//                server.receiveBinMask(binMask);
//                imshow(BINARY_MASK_R,binMask);
//            }
//            catch (ReceiveException& e)
//            {
//                if(e.error!=boost::asio::error::eof)
//                    throw e;
//                else
//                    server.acceptConnection();
//            }
//
//            waitKey(1000/FPS);
//
//        }
//    }
//    catch (std::exception& e)
//    {
//        cerr << e.what() << endl;
//    }
//
//    return 0;
//}
