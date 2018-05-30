#include "ros/ros.h"
#include "sockencan_bridge/socketcan_to_topic.h"
#include "socketcan_interface/filter.h" /*create a vector of filters, to filter out messages */
#include "socketcan_interface/socketcan.h" /*create a threaded interface */
#include <can_msgs/Frame.h>



int main(){


    /*
     * initializing ROS
     * TODO: tex file
     *
     */
    ros::init(argc, argv, "CAN_Node");

    /*
     * create a node handle and a parameter node handle
     * for clarification on multiplle node handle read : https://answers.ros.org/question/218348/what-is-the-use-of-multiple-nodehandles/
     */

    ros::NodeHandle nh(""), nh_param("~"); // also creates a parameter server using ~
    //uint32_t can_id = "878754";  /*whatever id has to be filter */


    /*
     * TODO: create a ros parameter using xmlrpc for rasparameter server
     * TODO: the parameter is directly used in the class to find the filter name or value
     *
     */

    //ros::param::set("/tc", 0x1f);
    //nh.setParam("can_ids",{"0x21", "0x34", 0x4f };
    // below can id is uint32_t, use type conversion from hex if else


    can::FrameFilterSharedPtr tofilter = boost::shared_ptr< can::FrameMaskFilter >(can_id); //polymorphic properties are used

    /*
     * create a vector of the filter to pass it to the setup
     *
     *
     */

    vector <can::FrameFilterSharedPtr> filtervector;
    filtervector.push(tofilter);

    /*
     *
     * create an interface for the driver
     * here we set device as can0
     */

    std::string can_device;
    nh_param.param<std::string>("can_device", can_device, "can0");
    can::ThreadedSocketCANInterfaceSharedPtr driver = boost::make_shared<can::ThreadedSocketCANInterface> ();


    /*check if the interface works as is required
     */



    if (!driver->init(can_device, 0))  // initialize device at can_device, 0 for no loopback.
    {
        ROS_FATAL("Failed to initialize can_device at %s", can_device.c_str());
        return 1;
    }
    else
    {
        ROS_INFO("Successfully connected to %s.", can_device.c_str());
    }





    /* create an object from socketcan_bridge
     * method is declared in sockencan_bridge/socketcan_to_topic.h
     * subscribes to can0 for the messages and publishes ros messages to itself
     */
    socketcan_bridge::SocketCANToTopic socketCANobject(&nh,&nh_param,driver);


    /*
     *
     * Here we send the interface to the setup, it goes to the call back
     *
     * and does the following
     *
     * 1) verifies legality of the messaage
     * 2) publishes to the node the can messages
     *
     * the argument to the setup is setup(const can::FilteredFrameListener::FilterVector &filters)
     *
     */

    socketCANobject.setup(filtervector);


    /*publish the messages*/



    /*ros spin at certain hz, baudrate set default is 250k
     * driver interface is shutdown
     * then reset
     * etc.,.
     */


    ros::spin();

    driver->shutdown();
    driver.reset();

    ros::waitForShutdown();



}