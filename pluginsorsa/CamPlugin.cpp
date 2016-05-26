//**********************************************************************************************************
//
// File: CamPlugin.cpp
//
//**********************************************************************************************************

class CCamPlugin : public MCamPlugin
{
public:
    CCamPlugin() 
        : iModel( 0 )
    {
    }
    
    virtual ~CCamPlugin() 
    {
        iAppUi->RemoveView( iPreview->Id() );               // Delete views from memory
        iAppUi->RemoveView( iInspect->Id() );
    }
    
    void ConstructL( MCamModelPlugin& aModel,               // Initialize plugin
                     CAknViewAppUi& aAppUi )
    {
        iAppUi = &aAppUi;
        iModel = &aModel;

        iPreview = new(ELeave) CViewPreview;         	    // Initialize preview view
        iPreview->SetModel( iModel );
        iPreview->ConstructL();
        iAppUi->AddViewL( iPreview );

        iInspect = new(ELeave) CCamPluginViewInspect;       // Initialize inspect view
        iInspect->SetModel( iModel );
        iInspect->ConstructL();
        iAppUi->AddViewL( iInspect );
              
        iPreview->SetInspectView( iInspect );               // Store inspect view pointer into preview view
                                                            // for view switching.
    }

    TCamPluginAction ExecuteL()                             // Plugin execution
    {
        if( iModel->HasBitmapHigh() )
        {
            User::Leave( KErrGeneral );                     // Fail if no VGA-image available.
        }
        else
        {
            iPreview->ActivateL();                          // Activate preview view

        }
        CActiveScheduler::Start();                          // Start executing plugin.
        return iModel->Action();                            // Return to main program.
    }
private:
    CAknViewAppUi* iAppUi;                                  // For view handling
    MCamModelPlugin* iModel;                                // Model class
    CViewPreview* iPreview;                                 // Our own preview view 
    CCamPluginViewInspect* iInspect;                        // Inspect view form view package

};

// DLL entry points follow:

GLDEF_C TInt E32Dll( TDllReason /*aReason*/ )               // The E32Dll() entry point function 
{                                                           // for all DLLs
    return KErrNone;
}

EXPORT_C CCamPluginInfo* QueryL()                           // Ordinal 1
{
    CResourceHelp* resHelp = CResourceHelp::NewLC( KResourceFile );
    HBufC* name = resHelp->ReadResourceStringLC( R_PLUGIN_NAME );
    HBufC* description = resHelp->ReadResourceStringLC( R_PLUGIN_DESCRIPTION );

    CCamPluginInfo* info = new(ELeave) CCamPluginInfo;    // New info object
    CleanupStack::PushL( info );
    
    info->ConstructL( *name,                              // Fill info object
                      *description,
                      KCamPluginAPIVersion,
                      TVersion( 1, 0, 1 ),
                      1,
                      KCamPluginPreview,                  // Capability flags
                      KPluginOrder,                       // Plugin order constant
                      KInstallSis );
    CleanupStack::Pop(); // info
    CleanupStack::PopAndDestroy( 3 ); // description, name, resHelp
    return info;
}

EXPORT_C MCamPlugin* CreateL()                              // Ordinal 2
{
    return new(ELeave) CCamPlugin;
}
