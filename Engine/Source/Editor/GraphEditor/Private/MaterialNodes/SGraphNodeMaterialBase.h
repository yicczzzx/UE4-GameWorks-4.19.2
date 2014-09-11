// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

typedef TSharedPtr<class FPreviewElement, ESPMode::ThreadSafe> FThreadSafePreviewPtr;

class FPreviewViewport : public ISlateViewport
{
public:
	FPreviewViewport(class UMaterialGraphNode* InNode);
	~FPreviewViewport();

	// ISlateViewport interface
	virtual void OnDrawViewport( const FGeometry& AllottedGeometry, const FSlateRect& MyClippingRect, class FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled ) override;
	virtual FIntPoint GetSize() const override;
	virtual class FSlateShaderResource* GetViewportRenderTargetTexture() const override {return NULL;}
	virtual bool RequiresVsync() const override {return false;}

	/** Material node to get expression preview from */
	UMaterialGraphNode* MaterialNode;
	/** Custom Slate Element to display the preview */
	FThreadSafePreviewPtr PreviewElement;
};

class FPreviewElement : public ICustomSlateElement
{
public:
	FPreviewElement();
	~FPreviewElement();

	/**
	 * Sets up the canvas for rendering
	 *
	 * @param	InCanvasRect			Size of the canvas tile
	 * @param	InClippingRect			How to clip the canvas tile
	 * @param	InExpressionPreview		Render proxy for the Material preview
	 * @param	bInIsRealtime			Whether preview is using realtime values
	 *
	 * @return	Whether there is anything to render
	 */
	bool BeginRenderingCanvas( const FIntRect& InCanvasRect, const FIntRect& InClippingRect, FMaterialRenderProxy* InExpressionPreview, bool bInIsRealtime );

private:
	/**
	 * ICustomSlateElement interface 
	 */
	virtual void DrawRenderThread(FRHICommandListImmediate& RHICmdList, const void* InWindowBackBuffer) override;

private:
	/** Render target that the canvas renders to */
	class FSlateMaterialPreviewRenderTarget* RenderTarget;
	/** Render proxy for the expression preview */
	FMaterialRenderProxy* ExpressionPreview;
	/** Whether preview is using realtime values */
	bool bIsRealtime;
};

class SGraphNodeMaterialBase : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SGraphNodeMaterialBase){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, class UMaterialGraphNode* InNode);

	// SGraphNode interface
	virtual void CreatePinWidgets() override;
	// End of SGraphNode interface

	// SNodePanel::SNode interface
	virtual void MoveTo(const FVector2D& NewPosition, FNodeSet& NodeFilter) override;
	// End of SNodePanel::SNode interface

	UMaterialGraphNode* GetMaterialGraphNode() const {return MaterialNode;}

	/* Populate a meta data tag with information about this graph node */
	virtual void PopulateMetaTag(class FGraphNodeMetaData* TagMeta) const override;

protected:
	// SGraphNode interface
	virtual void AddPin( const TSharedRef<SGraphPin>& PinToAdd ) override;
	virtual void CreateBelowPinControls(TSharedPtr<SVerticalBox> MainBox) override;
	virtual void SetDefaultTitleAreaWidget(TSharedRef<SOverlay> DefaultTitleAreaWidget) override;
	virtual TSharedRef<SWidget> CreateNodeContentArea() override;
	// End of SGraphNode interface

	/** Creates a preview viewport if necessary */
	TSharedRef<SWidget> CreatePreviewWidget();

	/** Returns visibility of Expression Preview viewport */
	EVisibility ExpressionPreviewVisibility() const;

	/** Show/hide Expression Preview */
	void OnExpressionPreviewChanged( const ESlateCheckBoxState::Type NewCheckedState );

	/** hidden == unchecked, shown == checked */
	ESlateCheckBoxState::Type IsExpressionPreviewChecked() const;

	/** Up when shown, down when hidden */
	const FSlateBrush* GetExpressionPreviewArrow() const;

private:
	/** Slate viewport for rendering preview via custom slate element */
	TSharedPtr<FPreviewViewport> PreviewViewport;

	/** Cached material graph node pointer to avoid casting */
	UMaterialGraphNode* MaterialNode;
};
