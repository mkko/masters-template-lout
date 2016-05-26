//**********************************************************************************************************
//
// File: ContainerPreview.cpp
//
//**********************************************************************************************************

// Graphic layout settings
const TInt KTextYMargin = 2;

void CContainerPreview::ConstructL( const TRect& aRect )
{
    iResourceHelp = CResourceHelp::NewL( KResourceFile );   // Initialize resource utility

    iGoingToPostProcess = EFalse;                           // Set mode to normal operation
    iError = EFalse;                                        // No errors
    iSetting = KSettingNormal;                              // Set plugin setting to default value

    CCamPluginContainerPreview::ConstructL( aRect );        // Initialize parent container

    //
    // Plugin specific initialization here
    //

    ShowScrollBarL( KSettingHigh+1, 1, KSettingHigh );      // Set up the scroll indicator
}

CContainerPreview::~CContainerPreview()
{
    delete iResourceHelp;
}

void CContainerPreview::FocusChanged( TDrawNow /*aDrawNow*/ )
{
    if( IsFocused() )
    {
        ShowScrollBarL( KSettingHigh+1, 1, KSettingHigh - iSetting );
    }
}

void CContainerPreview::Draw( const TRect& aRect ) const
{
    if( iError )
    {
        CWindowGc& gc = SystemGc();                         // Draw white screen in error mode.
        gc.SetPenStyle( CGraphicsContext::ENullPen );
        gc.SetBrushColor( KRgbWhite) ;
        gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
        gc.DrawRect( aRect );
        return;
    }
    if( StandBy() )
    {
        DrawStandByImage( aRect );                         // Let parent container handle standby gfx.
    } 
    else if( iGoingToPostProcess == EFalse )               // Normal operating mode
    {
        CWindowGc& gc = SystemGc();
        CFbsBitmap* bitmap = Model()->BitmapLow();
        if( bitmap && bitmap->Handle() )                   // Draw QQVGA bitmap if available
        {
	    TSize size = bitmap->SizeInPixels();
            TPoint dest( aRect.iTl.iX + ( aRect.Width() - size.iWidth )/2,
                         aRect.iTl.iY + KTopMargin );

            ClearBorders( aRect, dest, size );
            DrawBorderLines( dest, size );

            gc.BitBlt( dest, bitmap ); 

            DrawFreeMem( TPoint( dest.iX + size.iWidth, 
                                 dest.iY ) );

            gc.SetPenStyle( CGraphicsContext::ENullPen );
            gc.SetBrushStyle( CGraphicsContext::ESolidBrush );

            gc.SetBrushColor( KRgbWhite );
            gc.SetPenColor( KRgbBlack );
            const CFont* font = LatinPlain12();
            gc.UseFont( font );

            TInt xOffset = Size().iWidth / 2 
                - font->TextWidthInPixels( text ) / 2;
            
	    //
	    // Draw text about plugin settings here.
	    //

            gc.DrawText( _L("K-P was here 9.3.2004!"),
                         TPoint( xOffset,
                                 KTextYMargin + dest.iY 
                                 + size.iHeight + font->AscentInPixels() ) );
        }
        iModel->SnapLowL();                                 // Request another QQVGA image
    }
}



TKeyResponse
CContainerPreview::OfferKeyEventL( const TKeyEvent& aKeyEvent,
                                   TEventCode aType )
{
    if( Busy() || StandBy() ) 
    {
        return EKeyWasConsumed;                             // No response when busy or in stand by mode.
    }
    if( aType != EEventKey )
    {
        return EKeyWasNotConsumed;
    }
    switch( aKeyEvent.iCode )
    {
        case EKeyDownArrow:
            iSetting -= KSettingStep;
            if( iSetting < KSettingLow )
            {
                iSetting = KSettingHigh;
            }
            MoveScrollBarTo( KSettingHigh - iSetting );
            return EKeyWasConsumed;
        case EKeyUpArrow:
            iSetting += KSettingStep;
            if( iSetting > KSettingHigh )
            {
                iSetting = KSettingLow;
            }
            MoveScrollBarTo( KSettingHigh - iSetting );
            return EKeyWasConsumed;
        case EKeyOK:
            SnapL();
            return EKeyWasConsumed;
        default:
            break;
    }
    return CCamPluginContainerPreview::OfferKeyEventL( aKeyEvent,
                                                       aType );
}

void CContainerPreview::SnapL()
{
    SetBusy();                                              // Set the busy status.
    NaviTextHoldStillL();                                   // Display information for the user.
    iGoingToPostProcess = ETrue;                            // Change away from normal mode.
    Model()->SnapHighL();                                   // Request VGA image.
}


void CContainerPreview::ImageSnapped( TInt aError )
{
    if( aError != KErrNone )
    {
        ClearBusy();                                        // Error while snapping VGA image.
        iGoingToPostProcess = EFalse;                       // Go to normal mode.
        iError = ETrue;                                     // Set error status.
        if( aError == KErrNoMemory )                        // Display error message.
        {
            ErrorNotePluginOutOfMemoryL();
        }
        else
        {
            ErrorNoteImageSnapL();
        }
        return;
    }
    if( iGoingToPostProcess )                              
    {
        SetBusy();                                          // VGA image snapped ok.
        NaviTextProcessingL();                              // Display informative text.

        TRAPD( err,
        {
            Model()->ApplyCorrectionsToBitmapHighL();       // First apply corrections to the image.
	    //
            // Plugin specific image processing here for the VGA image
	    //
        } ); // TRAPD
        if( err != KErrNone )
        {
            ClearBusy();                                    // Something failed.
            NaviTextClearL();                               // Clear status text.
            iGoingToPostProcess = EFalse;                   // Go to normal mode.
            ErrorNotePluginOutOfMemoryL();                  // Show error message.
            return;
        }
        Model()->ScaleHighToLowL();                         // Then create a LQ image from the HQ one
    }
    else
    {
	//
	// Plugin specific image processing here for the QQVGA image
	//
        DrawDeferred();                                    // Normal operating mode, draw the QQVGA image.
    }
}

void CContainerPreview::ImageProcessed( TInt aError )
{
    if( aError != KErrNone ) 
    {
        ClearBusy();                                        // Error processing image.
        NaviTextClearL();                                   // Clear status text.
        ErrorNoteImageProcessL();                           // Display error message.
        Model()->SetBitmapHigh( 0 );                        // Dispose of the VGA image.
        iGoingToPostProcess = EFalse;                       // And go to normal operating mode.
        DrawDeferred();
        return;
    }
    NaviTextSavingImageL();                                 // All ok, display informative text.
    Model()->SaveToPhotoAlbumL();                           // Start saving VGA image to the photoalbum.
}

void CContainerPreview::ImageSaved( TInt aError )
{
    ClearBusy();                                            // Image saved.
    NaviTextClearL();                                       // Clear status text.
    if( aError != KErrNone )
    {
        ErrorNoteImageSaveL();                              // Save failed, display error message.
        Model()->SetBitmapHigh( 0 );                        // Get rid of the VGA image.
        iGoingToPostProcess = EFalse;                       // Go to normal mode.
        DrawDeferred();
        return;
    }
    ActivateInspectViewL();                                 // Image save ok, switch to inspect view.
}



  
