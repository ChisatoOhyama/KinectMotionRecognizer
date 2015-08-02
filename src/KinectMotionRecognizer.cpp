// -*- C++ -*-
/*!
 * @file  KinectMotionRecognizer.cpp
 * @brief Recognize  Motion and Output Motion Label (String)
 * @date $Date$
 *
 * $Id$
 */

#include "KinectMotionRecognizer.h"

// Module specification
// <rtc-template block="module_spec">
static const char* kinectmotionrecognizer_spec[] =
  {
    "implementation_id", "KinectMotionRecognizer",
    "type_name",         "KinectMotionRecognizer",
    "description",       "Recognize  Motion and Output Motion Label (String)",
    "version",           "1.0.0",
    "vendor",            "Ogata Laboratory",
    "category",          "recognizer",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.debug", "0",
    // Widget
    "conf.__widget__.debug", "text",
    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
KinectMotionRecognizer::KinectMotionRecognizer(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_skeletonIn("skeleton", m_skeleton),
    m_motionOut("motion", m_motion)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
KinectMotionRecognizer::~KinectMotionRecognizer()
{
}



RTC::ReturnCode_t KinectMotionRecognizer::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("skeleton", m_skeletonIn);
  
  // Set OutPort buffer
  addOutPort("motion", m_motionOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("debug", m_debug, "0");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t KinectMotionRecognizer::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t KinectMotionRecognizer::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t KinectMotionRecognizer::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t KinectMotionRecognizer::onActivated(RTC::UniqueId ec_id)
{
	m_Mode = RECOGNITION_MODE;
  return RTC::RTC_OK;
}


RTC::ReturnCode_t KinectMotionRecognizer::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t KinectMotionRecognizer::onExecute(RTC::UniqueId ec_id)
{
	if(m_skeletonIn.isNew()) {
		m_skeletonIn.read();
		//std::cout << "Kinect Skeleton Data Arrived.\n";

		if(m_Mode == RECOGNITION_MODE){
			for(int i = 0;i < 6;i++) {
				if(m_skeleton.SkeletonData[i].trackingState == KINECT::NUI_SKELETON_TRACKED) {
					float head_x = m_skeleton.SkeletonData[i].skeletonPositions[KINECT::NUI_SKELETON_POSITION_HEAD].v[0];
					float head_y = m_skeleton.SkeletonData[i].skeletonPositions[KINECT::NUI_SKELETON_POSITION_HEAD].v[1];
					float head_z = m_skeleton.SkeletonData[i].skeletonPositions[KINECT::NUI_SKELETON_POSITION_HEAD].v[2];
					
					float left_x = m_skeleton.SkeletonData[i].skeletonPositions[KINECT::NUI_SKELETON_POSITION_HAND_LEFT].v[0];
					float left_y = m_skeleton.SkeletonData[i].skeletonPositions[KINECT::NUI_SKELETON_POSITION_HAND_LEFT].v[1];
					float left_z = m_skeleton.SkeletonData[i].skeletonPositions[KINECT::NUI_SKELETON_POSITION_HAND_LEFT].v[2];
					
					float right_x = m_skeleton.SkeletonData[i].skeletonPositions[KINECT::NUI_SKELETON_POSITION_HAND_RIGHT].v[0];
					float right_y = m_skeleton.SkeletonData[i].skeletonPositions[KINECT::NUI_SKELETON_POSITION_HAND_RIGHT].v[1];
					float right_z = m_skeleton.SkeletonData[i].skeletonPositions[KINECT::NUI_SKELETON_POSITION_HAND_RIGHT].v[2];
					
					if(left_z>head_z && right_z>head_z){
						std::cout << "BAN_(^o^)^ZAI" << std::endl;
						m_motion.data= "banzai";
						m_motionOut.write();
						m_StartTime = coil::gettimeofday();
						m_Mode = SPEACH_MODE;
					}else if(right_x<head_x){
						std::cout << "HEN_i_žP jSHIN" << std::endl;
						m_motion.data = "henshin";
						m_motionOut.write();
						m_StartTime = coil::gettimeofday();
						m_Mode = SPEACH_MODE;
					}else{
						m_motion.data = "‚È‚µ";
						std::cout << "nothing" << std::endl;
						m_motionOut.write();
						m_StartTime = coil::gettimeofday();
						m_Mode = SPEACH_MODE;
					}
				}
			}
		}else if(m_Mode == SPEACH_MODE){
			double duration = coil::gettimeofday() - m_StartTime;
			if(duration  > 2){
				m_Mode = RECOGNITION_MODE;
			}
		}


			
	}

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t KinectMotionRecognizer::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t KinectMotionRecognizer::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t KinectMotionRecognizer::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t KinectMotionRecognizer::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t KinectMotionRecognizer::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void KinectMotionRecognizerInit(RTC::Manager* manager)
  {
    coil::Properties profile(kinectmotionrecognizer_spec);
    manager->registerFactory(profile,
                             RTC::Create<KinectMotionRecognizer>,
                             RTC::Delete<KinectMotionRecognizer>);
  }
  
};


