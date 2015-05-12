#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"

#include "cinder/Rand.h"
#include "cinder/Capture.h"

#include "CinderOpenCv.h"

#include "markerdetector.h"
#include "cvdrawingutils.h"

using namespace ci;
using namespace ci::app;

class SimpleCornerDetectionApp : public AppNative {
  public:
   void prepareSettings( Settings *settings );
	void setup();
	void update();
	void draw();
    
    gl::TextureRef          mInput;
    Surface             	mSurf;
    CaptureRef              mCapture;
    
    //aruco::CameraParameters mCamParam;
    aruco::MarkerDetector mMarkerDetector;
    std::vector<aruco::Marker> mMarkers;
};
void SimpleCornerDetectionApp::prepareSettings( cinder::app::AppBasic::Settings *settings ) {
    settings->setFrameRate( 60.0 );
    settings->setWindowSize( 640, 480 );
}
void SimpleCornerDetectionApp::setup()
{
	// CAMERA
    try {
		mCapture = Capture::create( 640, 480 );
		mCapture->start();
	}catch( ... ) {
		console() << "Failed to initialize capture" << std::endl;
	}
}   
void SimpleCornerDetectionApp::update(){
    if( mCapture->checkNewFrame() ){
        mSurf = mCapture->getSurface();
        mInput = gl::Texture::create( mSurf );
    }
    if( mSurf ){
        cv::Mat input = toOcv( mSurf );
        mMarkerDetector.detect( input, mMarkers );
    }
}
void SimpleCornerDetectionApp::draw()
{

	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    gl::enableAlphaBlending();
    gl::color( ColorA(1,1,1,1.0f) );
    if( mInput ){
        gl::draw( mInput );
    }
    gl::color( ColorA(1,1,1,0.7f) );
    for( int i=0; i<mMarkers.size(); i++){
        for( int ii = 0; ii < mMarkers[i].size(); ii++){
            gl::drawSolidCircle(Vec2f(mMarkers[i][ii].x, mMarkers[i][ii].y), 10.0f);
        }
    }
}

CINDER_APP_NATIVE( SimpleCornerDetectionApp, RendererGl )
