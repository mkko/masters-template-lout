//**********************************************************************************************************
//
// File: ViewPreview.h
//
//**********************************************************************************************************

/**
 * Plugin preview view
 */
class CViewPreview : public CCamPluginViewPreview
{
public: 
    /**
     * View constructor.
     * Adds plugin resource file in resource path.
     */
    void ConstructL();

    /// Destructor
    virtual ~CViewPreview();

    /**
     * Handles view commands.
     */
    void HandleCommandL( TInt aCommand );

    /**
     * View activation, creates container.
     */
    void DoActivateL( const TVwsViewId& aPrevViewId,
                      TUid aCustomMessageId,
                      const TDesC8& aCustomMessage );
private:
};

