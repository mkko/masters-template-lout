//**********************************************************************************************************
//
// File: ContainerPreview.cpp
//
//**********************************************************************************************************

void CViewPreview::ConstructL()
{
    CCamPluginViewPreview::ConstructL();
}

CViewPreview::~CViewPreview()
{
}

void CViewPreview::DoActivateL( const TVwsViewId& /*aPrevViewId*/,
				TUid /*aCustomMessageId*/,
				const TDesC8& /*aCustomMessage*/ )
{
    if( !iContainer )
    {
        CContainerPreview* container = new(ELeave) CContainerPreview;  // Create the container anew
        InitializeContainer( container );                   // View-package initialization
	iContainer = container;
        iContainer->SetMopParent( this );                   
        iContainer->ConstructL( ClientRect() );
        AppUi()->AddToStackL( *this, iContainer );          // Add container to control stack
    } 
    iContainer->MakeVisible( ETrue );
}

void CViewPreview::HandleCommandL( TInt aCommand )
{   
    if( iContainer->Busy() )                               // No response to user events when in busy mode.
    {
        return;
    }
    switch ( aCommand )
    {
        case ECamPluginCmdCapture:
            ((CContainerPreview*)iContainer)->SnapL();      // Only react to one event.
            break;
        default:
            CCamPluginViewPreview::HandleCommandL( aCommand ); // Pass everything else to the parent class.
            break;
    }
}
