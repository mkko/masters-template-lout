//**********************************************************************************************************
//
// File: ContainerPreview.cpp
//
//**********************************************************************************************************

/**
 * Plugin preview container
 */
class CContainerPreview : public CCamPluginContainerPreview
{
public: 
    /**
     * Container constructor.
     * Creates an instance of the plugin functions class.
     * Creates a delayed snap timer utility.
     * @param aRect Container rectangle
     */
    void ConstructL( const TRect& aRect );

    /// Destructor
    virtual ~CContainerPreview();

    /**
     * Draws the LQ image from camera.
     * Prints the setting text with arrow indicators.
     * Asks for another picture from camera if everything is ok.
     * Draws standby screen if in standby-mode.
     * @param aRect Rectangle to draw
     */
    void Draw( const TRect& aRect ) const;

    /**
     * Handles plugin key events.
     * @param aKeyEvent Key event data
     * @param aType Event type
     */
    TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, 
                                 TEventCode aType );

    /**
     * Notify call when container focus changed.
     * Displays scroll indicators when gaining focus.
     * @param TDrawNow
     */
    void FocusChanged( TDrawNow aDrawNow );
    /**
     * Start snapping an HQ image.
     */
     void SnapL();
    /** 
     * From MImageSnapObserver. Called when an image
     * from camera is ready. Displays an error note in case of an 
     * error. Processes the given image with plugin functions.
     * If an HQ image was snapped, starts to create a LQ version of it.
     */
    void ImageSnapped( TInt aError );
    /**
     * From MImageSnapObserver. Called when image processing
     * is complete. Displays an error note in case of an error.
     * Starts saving image to photo album.
     */
    void ImageProcessed( TInt aError );
    /**
     * From MImageSaveObserver. Called when image has been saved.
     * Displays an error note in case of an error.
     * Activates inspect view if everything ok.
     */
    void ImageSaved( TInt aError );

private:
    /**
     * If ETrue, the next image snap is an HQ snap. After an HQ snap,
     * the image should be processed and saved to the photoalbum.
     * If EFalse, LQ images will be capture from camera and displayed 
     * repeatedly on screen.
     */
    TBool iGoingToPostProcess;
    /**
     * If ETrue, an error has been occured, and LQ image capturing
     * is stopped.
     */
    TBool iError;
    /// Current setting.
    TInt iSetting;
    /// Resource utility
    CResourceHelp* iResourceHelp;
};

#endif



