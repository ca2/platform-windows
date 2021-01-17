#pragma once


namespace draw2d_gdiplus
{


   class CLASS_DECL_DRAW2D_GDIPLUS graphics :
      virtual public ::draw2d::graphics
   {
   public:


      ::Gdiplus::Matrix *           m_pm;
      ::Gdiplus::Graphics *         m_pgraphics;
      ::Gdiplus::GraphicsPath *     m_ppath;
      ::Gdiplus::GraphicsPath *     m_ppathPaint;
      HDC                           m_hdc;
      HDC                           m_hdcAttach;
      HDC                           m_hdcGraphics;


      graphics();
      virtual ~graphics();


      virtual void close_graphics();

      Gdiplus::Graphics * g()
      {
         //if(m_pgraphics == nullptr)
         //{
         //   m_pgraphics = ::Gdiplus::Graphics::FromHDC(get_handle1());
         //}
         return m_pgraphics;
      }

//      ::user::interaction_impl * GetWindow() override;

      virtual bool attach(void * pgraphics) override;   // attach/detach affects only the Output DC
      virtual void * detach() override;

      virtual bool attach_hdc(HDC hdc) override;   // attach/detach affects only the Output DC
      virtual HDC detach_hdc() override;
      virtual bool AttachPrinter(HDC hdc) override;
      virtual HDC DetachPrinter() override;


      bool IsPrinting() override;            // TRUE if being used for printing


      // for bidi and mirrored localization
      u32 GetLayout() override;
      u32 SetLayout(u32 dwLayout) override;


      virtual ::e_status set(::draw2d::bitmap* pbitmap) override;

      HDC get_hdc();
      void release_hdc(HDC hdc);


      virtual double get_dpix() override;
      virtual double get_dpiy() override;

      // Constructors
      bool CreateDC(const char * pszDriverName, const char * pszDeviceName,

                    const char * pszOutput, const void * lpInitData) override;

      bool CreateIC(const char * pszDriverName, const char * pszDeviceName,

                    const char * pszOutput, const void * lpInitData) override;

      bool CreateCompatibleDC(::draw2d::graphics * pgraphics) override;

      bool DeleteDC() override;

      // Device-Context Functions
      virtual i32 SaveDC() override;
      virtual bool RestoreDC(i32 nSavedDC) override;
      i32 GetDeviceCaps(i32 nIndex) override;
      virtual ::u32 SetBoundsRect(const ::rectd & rectBounds, ::u32 flags);
      virtual ::u32 GetBoundsRect(::rectd * rectdBounds, ::u32 flags);

      bool ResetDC(const DEVMODE* pDevMode) override;


      // Drawing-Tool Functions
      pointd GetBrushOrg() override;
      pointd SetBrushOrg(double x, double y) override;
      pointd SetBrushOrg(const ::pointd & point) override;
      //i32 EnumObjects(i32 nObjectType, i32 (CALLBACK* pfn)(LPVOID, LPARAM), LPARAM lpData) override;





      //::color GetNearestColor(const ::color & color) override;
      //::draw2d::palette* SelectPalette(::draw2d::palette* pPalette, bool bForceBackground) override;
      //UINT RealizePalette() override;
      //void UpdateColors() override;

      i32 GetPolyFillMode() override;
      i32 GetROP2() override;
      i32 GetStretchBltMode() override;

      bool _get(::draw2d::matrix & matrix) override;
      bool _set(const ::draw2d::matrix & matrix) override;
      //bool append(const ::draw2d::matrix & matrix) override;
      //bool prepend(const ::draw2d::matrix & matrix) override;

      i32 SetPolyFillMode(i32 nPolyFillMode) override;
      i32 SetROP2(i32 nDrawMode) override;
      //bool set_interpolation_mode(::draw2d::enum_interpolation_mode einterpolationmode) override;

      //bool GetColorAdjustment(LPCOLORADJUSTMENT pColorAdjust) override;

      //bool SetColorAdjustment(const COLORADJUSTMENT* pColorAdjust) override;


//#if (_WIN32_WINNT >= 0x0500)
//
//      ::color GetDCBrushColor() override;
//      ::color SetDCBrushColor(const ::color & color) override;
//
//      ::color GetDCPenColor() override;
//      ::color SetDCPenColor(const ::color & color) override;
//
//#endif

      // Graphics mode
      i32 SetGraphicsMode(i32 iMode) override;
      i32 GetGraphicsMode() override;

      // World transform
      bool SetWorldTransform(const XFORM* pXform) override;
      bool ModifyWorldTransform(const XFORM* pXform,u32 iMode) override;
      bool GetWorldTransform(XFORM* pXform) override;

      // Mapping Functions
      virtual i32 GetMapMode() override;
      //virtual point GetViewportOrg() override;
      virtual i32 SetMapMode(i32 nMapMode) override;
      // Viewport Origin
      //virtual point SetViewportOrg(double x, double y) override;
      //virtual point SetViewportOrg(const ::point & point) override;
      //virtual point OffsetViewportOrg(i32 nWidth, i32 nHeight) override;

      // Viewport Extent
      virtual sized GetViewportExt() override;
      virtual sized SetViewportExt(double cx, double cy) override;
      virtual sized SetViewportExt(const ::sized & size) override;
      //virtual size ScaleViewportExt(i32 xNum, i32 xDenom, i32 yNum, i32 yDenom) override;

      // Window Origin
      pointd GetWindowOrg() override;
      pointd SetWindowOrg(double x, double y) override;
      pointd SetWindowOrg(const ::pointd & point) override;
      pointd offset_window_org(double nWidth, double nHeight) override;

      // Window extent
      virtual sized GetWindowExt() override;
      virtual sized set_window_ext(double cx, double cy) override;
      virtual sized set_window_ext(const ::sized & size) override;
      virtual sized scale_window_ext(double xNum, double xDenom, double yNum, double yDenom) override;

      // Coordinate Functions
      void DPtoLP(::pointd * pPoints,count nCount = 1) override;

      void DPtoLP(::rectd * prect) override;

      void DPtoLP(::sized * pSize) override;

      void LPtoDP(::pointd * pPoints,count nCount = 1) override;

      void LPtoDP(::rectd * prect) override;

      void LPtoDP(::sized * pSize) override;


      // Special Coordinate Functions (useful for dealing with metafiles and OLE)
      void DPtoHIMETRIC(::sized * pSize) override;

      void LPtoHIMETRIC(::sized * pSize) override;

      void HIMETRICtoDP(::sized * pSize) override;

      void HIMETRICtoLP(::sized * pSize) override;


      // Region Functions
      bool fill_region(::draw2d::region* pregion, ::draw2d::brush* pBrush) override;
      bool frame_region(::draw2d::region* pregion, ::draw2d::brush* pBrush, double nWidth, double nHeight) override;
      bool invert_region(::draw2d::region* pregion) override;
      bool paint_region(::draw2d::region* pregion) override;

      // Clipping Functions
      virtual i32 get_clip_box(::rectd * prect) override;


      virtual ::e_status add_shapes(const shape_array& shapea);
      virtual ::e_status reset_clip();
      virtual ::e_status intersect_clip(const ::rect& rect);
      virtual ::e_status intersect_clip(const ::rectd& rect);
      virtual ::e_status intersect_clip(const ::oval& oval);
      virtual ::e_status intersect_clip(const ::ovald& oval);
      virtual ::e_status intersect_clip(const ::polygon& polygon);
      virtual ::e_status intersect_clip(const ::polygond& polygon);

      //virtual bool PtVisible(double x, double y) override;
      //bool PtVisible(const ::point & point) override;
      //virtual bool RectVisible(const rect & prect) override;

      //i32 SelectClipRgn(::draw2d::region* pRgn) override;
      //i32 ExcludeClipRect(i32 x1, i32 y1, i32 x2, i32 y2) override;
      //i32 ExcludeClipRect(const rect &  prect) override;



//      i32 ExcludeUpdateRgn(::user::primitive * pwindow) override;
      //i32 IntersectClipRect(i32 x1, i32 y1, i32 x2, i32 y2) override;
      //i32 IntersectClipRect(const rect &  prect) override;

      //i32 OffsetClipRgn(double x, double y) override;
      //i32 OffsetClipRgn(const ::size & size) override;
      //i32 SelectClipRgn(::draw2d::region* pRgn, ::draw2d::enum_combine ecombine) override;

      // Line-Output Functions
      pointd current_position() override;
//      point move_to(double x, double y) override;
      //    point move_to(const ::point & point) override;
      using ::draw2d::graphics::line_to;
      virtual bool line_to(double x, double y) override;
      //bool line_to(const ::pointd& point) override;
      //  bool line_to(const ::point & point) override;
      virtual bool polyline(const ::pointd * ppoint,count nCount) override;


      using ::draw2d::graphics::Arc;
      //bool Arc(i32 x1,i32 y1,i32 x2,i32 y2,i32 x3,i32 y3,i32 x4,i32 y4) override;
      bool Arc(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4) override;

      //bool Arc(i32 x1,i32 y1,i32 x2,i32 y2, angle start, angle extends) override;
      bool Arc(double x1,double y1,double x2,double y2,angle start, angle extends) override;

      bool AngleArc(double x,double y,i32 nRadius, angle fStartAngle, angle fSweepAngle) override;
      //bool ArcTo(i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3, i32 x4, i32 y4) override;
      bool ArcTo(const rectd &  prect, const pointd & pointStart, const pointd & pointEnd) override;

      //i32 GetArcDirection() override;
      //i32 SetArcDirection(i32 nArcDirection) override;

      virtual bool polydraw(const ::pointd * ppoint, const BYTE* lpTypes, ::count nCount) override;

      virtual bool polyline_to(const ::pointd * ppoint,count nCount) override;

      virtual bool poly_polyline(const ::pointd * ppoint, const int * lpPolyPoints, ::count nCount) override;


      virtual bool poly_bezier(const ::pointd * ppoint, ::count nCount) override;

      virtual bool poly_bezier_to(const ::pointd * ppoint, ::count nCount) override;



      void frame_rect(const ::rectd & rect, ::draw2d::brush* pBrush) override;
      void invert_rect(const ::rectd & rect) override;
      //bool draw(const ::point & point, ::draw2d::icon * picon, const ::size & size) override;
      //bool DrawIcon(double x, double y, ::draw2d::icon * picon, i32 cx, i32 cy, UINT istepIfAniCur, HBRUSH hbrFlickerFreeDraw, UINT diFlags) override;
      //bool DrawState(const ::point & point, const ::size & size, HBITMAP hBitmap, UINT nFlags,
        //             HBRUSH hBrush = nullptr) override;
      //bool DrawState(const ::point & point, const ::size & size, ::draw2d::bitmap* pBitmap, UINT nFlags,
        //             ::draw2d::brush* pBrush = nullptr) override;
      //bool DrawState(const ::point & point, const ::size & size, HICON hIcon, UINT nFlags,
      //               HBRUSH hBrush = nullptr) override;
      //bool DrawState(const ::point & point, const ::size & size, HICON hIcon, UINT nFlags,
      //               ::draw2d::brush* pBrush = nullptr) override;
      //bool DrawState(const ::point & point, const ::size & size, const char * pszText, UINT nFlags,

      //               bool bPrefixText = TRUE, i32 nTextLen = 0, HBRUSH hBrush = nullptr) override;
      //bool DrawState(const ::point & point, const ::size & size, const char * pszText, UINT nFlags,

      //               bool bPrefixText = TRUE, i32 nTextLen = 0, ::draw2d::brush* pBrush = nullptr) override;
      //bool DrawState(const ::point & point, const ::size & size, DRAWSTATEPROC pDrawProc,

      //               LPARAM lData, UINT nFlags, HBRUSH hBrush = nullptr) override;
      //bool DrawState(const ::point & point, const ::size & size, DRAWSTATEPROC pDrawProc,

      //               LPARAM lData, UINT nFlags, ::draw2d::brush* pBrush = nullptr) override;

      // Ellipse and Polygon Functions
      //bool Chord(i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3,
        //         i32 x4, i32 y4) override;
      bool Chord(const rectd &  prect, const pointd & pointStart, const pointd & pointEnd) override;

      void DrawFocusRect(const rectd &  prect) override;


      //bool draw_ellipse(i32 x1, i32 y1, i32 x2, i32 y2) override;
      //bool draw_ellipse(const rect &  prect) override;

      //bool fill_ellipse(i32 x1, i32 y1, i32 x2, i32 y2) override;
      //bool fill_ellipse(const rect &  prect) override;

      virtual ::e_status clear_current_point() override;

      //bool draw_ellipse(double x1,double y1,double x2,double y2) override;
      bool draw_ellipse(const ::rectd & prect) override;

      //bool fill_ellipse(double x1,double y1,double x2,double y2) override;
      bool fill_ellipse(const ::rectd & prect) override;


      //bool Pie(i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3, i32 x4, i32 y4) override;
      bool Pie(const rectd &  prect, const pointd & pointStart, const pointd & pointEnd) override;


      //bool draw_polygon(const POINT * ppoints, count nCount) override;
      bool draw_polygon(const POINTD * ppoints, count nCount) override;
      //bool fill_polygon(const POINT * ppoints, count nCount) override;
      bool fill_polygon(const POINTD * ppoints, count nCount) override;
      using ::draw2d::graphics::rectangle;
      using ::draw2d::graphics::draw_rect;
      using ::draw2d::graphics::fill_rect;

      virtual bool rectangle(const ::rectd & rect) override;
      virtual bool draw_rect(const ::rectd & rect, ::draw2d::pen * ppen) override;
      virtual bool fill_rect(const ::rectd & rect, ::draw2d::brush * pbrush) override;

      virtual bool round_rect(const ::rectd & rect, double dRadius) override;


      virtual bool _draw_raw(const ::rectd & rectDst, ::draw2d::graphics * pgraphicsSrc, const ::rectd & rectSrc = ::rectd()) override;
      //virtual bool _stretch_raw(const ::rectd & rectDst, ::draw2d::graphics * pgraphicsSrc, const ::rectd & rectSrc = ::rectd()) override;


      //// Bitmap Functions
      //bool PatBlt(double x, double y, i32 nWidth, i32 nHeight, u32 dwRop) override;
      //bool BitBltRaw(double x, double y, i32 nWidth, i32 nHeight, ::draw2d::graphics * pgraphicsSrc,
      //               i32 xSrc, i32 ySrc, u32 dwRop) override;
      //bool StretchBltRaw(double x, double y, double nWidth, double nHeight, ::draw2d::graphics * pgraphicsSrc,
      //                   i32 xSrc, i32 ySrc, i32 nSrcWidth, i32 nSrcHeight, u32 dwRop) override;


      //bool StretchBltRaw(double x, double y, i32 nWidth, i32 nHeight, ::draw2d::graphics * pgraphicsSrc,
      //                   i32 xSrc, i32 ySrc, i32 nSrcWidth, i32 nSrcHeight, u32 dwRop) override;

      ::color GetPixel(double x, double y) override;
      ::color GetPixel(const ::pointd & point) override;
      ::color SetPixel(double x, double y, const ::color & color) override;
      ::color SetPixel(const ::pointd & point, const ::color & color) override;
      ::color blend_pixel(const ::pointd & point, const ::color & color) override;
      //bool FloodFill(double x, double y, const ::color & color) override;
      //bool ExtFloodFill(double x, double y, const ::color & color, UINT nFillType) override;
      //bool MaskBlt(double x, double y, i32 nWidth, i32 nHeight, ::draw2d::graphics * pgraphicsSrc,
      //             i32 xSrc, i32 ySrc, ::draw2d::bitmap& maskBitmap, i32 xMask, i32 yMask,
      //             u32 dwRop) override;
      //bool PlgBlt(LPPOINT pPoint, ::draw2d::graphics * pgraphicsSrc, i32 xSrc, i32 ySrc,

      //            i32 nWidth, i32 nHeight, ::draw2d::bitmap& maskBitmap, i32 xMask, i32 yMask) override;
      bool SetPixelV(double x, double y, const ::color & color) override;
      bool SetPixelV(const ::pointd & point, const ::color & color) override;
      //bool GradientFill(TRIVERTEX* pVertices, ULONG nVertices,
      //                  void * pMesh, ULONG nMeshElements, u32 dwMode) override;
      //bool TransparentBlt(i32 xDest, i32 yDest, i32 nDestWidth, i32 nDestHeight,
      //                    ::draw2d::graphics * pgraphicsSrc, i32 xSrc, i32 ySrc, i32 nSrcWidth, i32 nSrcHeight,
      //                    UINT clrTransparent) override;

      virtual bool _alpha_blend_raw(const ::rectd & rectDst, ::draw2d::graphics * pgraphicsSrc, const ::rectd & rectSrc, double dOpacity) override;

      /*bool alpha_blend(i32 xDest, i32 yDest, i32 nDestWidth, i32 nDestHeight,
        ::draw2d::graphics * pgraphicsSrc, i32 xSrc, i32 ySrc, i32 nSrcWidth, i32 nSrcHeight,
        BLENDFUNCTION blend) override;*/

      // Text Functions
      virtual bool TextOutRaw(double x, double y, const string & str) override;

      virtual bool ExtTextOut(double x, double y, UINT nOptions, const rectd &  prect, const char * pszString,strsize nCount, LPINT lpDxWidths) override;

      virtual bool ExtTextOut(double x, double y, UINT nOptions, const rectd &  prect, const string & str, LPINT lpDxWidths) override;

      virtual sized TabbedTextOut(double x, double y, const char * pszString, strsize nCount,count nTabPositions, LPINT lpnTabStopPositions, i32 nTabOrigin) override;

      virtual sized TabbedTextOut(double x, double y, const string & str,count nTabPositions, LPINT pnTabStopPositions, i32 nTabOrigin) override;


      //using ::draw2d::graphics::draw;
      //virtual bool draw(const ::point & point, ::draw2d::graphics * pgraphicsSrc, const ::rect & rectSrc) override;

      virtual bool _001DrawText(const string & str, rectd & rectParam, const ::e_align & ealign = e_align_top_left, const ::e_draw_text & edrawtext = e_draw_text_none, bool bMeasure = false) override;

      //virtual bool draw_text_ex(const char * pszString,strsize nCount,const ::rect & rect, const ::e_align & ealign = e_align_top_left, const ::e_draw_text & edrawtext = e_draw_text_none,LPDRAWTEXTPARAMS lpDTParams) override;

      //virtual bool draw_text_ex(const string & str,const ::rect & rect, const ::e_align & ealign = e_align_top_left, const ::e_draw_text & edrawtext = e_draw_text_none) override;


      //virtual bool draw_text_ex(const char * pszString,strsize nCount,const ::rectd & rect, const ::e_align & ealign = e_align_top_left, const ::e_draw_text & edrawtext = e_draw_text_none,LPDRAWTEXTPARAMS lpDTParams) override;

      virtual bool draw_text_ex(const string & str,const ::rectd & rect, const ::e_align & ealign = e_align_top_left, const ::e_draw_text & edrawtext = e_draw_text_none) override;

      virtual ::count get_character_extent(double_array & daLeft, double_array& daRight, const string & str, strsize iStart = 0, strsize iEnd = -1) override;

      virtual sized GetTextExtent(const char * pszString, strsize nCount, strsize iIndex) override;

      virtual sized GetTextBegin(const char * pszString, strsize nCount, strsize iIndex) override;

      //virtual sized GetTextExtent(const char * pszString, strsize nCount) override;

      virtual sized GetTextExtent(const string & str) override;
      virtual bool GetTextExtent(sized & size, const char * pszString, strsize nCount, strsize iIndex) override;

      virtual bool GetTextExtent(sized & size, const char * pszString, strsize nCount) override;

      virtual bool GetTextExtent(sized & size, const string & str) override;
      virtual sized GetOutputTextExtent(const char * pszString, strsize nCount) override;

      virtual sized GetOutputTextExtent(const string & str) override;
      virtual sized GetTabbedTextExtent(const char * pszString, strsize nCount,count nTabPositions, LPINT lpnTabStopPositions) override;

      virtual sized GetTabbedTextExtent(const string & str,count nTabPositions, LPINT pnTabStopPositions) override;

      virtual sized GetOutputTabbedTextExtent(const char * pszString, strsize nCount,count nTabPositions, LPINT lpnTabStopPositions) override;

      virtual sized GetOutputTabbedTextExtent(const string & str,count nTabPositions, LPINT pnTabStopPositions) override;

      //virtual bool GrayString(::draw2d::brush* pBrush, bool (CALLBACK* pfnOutput)(HDC, LPARAM, i32), LPARAM lpData, i32 nCount, double x, double y, i32 nWidth, i32 nHeight) override;

      virtual UINT GetTextAlign() override;
      virtual UINT SetTextAlign(UINT nFlags) override;
      //virtual i32 GetTextFace(count nCount, LPTSTR pszFacename) override;

      virtual i32 GetTextFace(string & rString) override;
      virtual bool get_text_metrics(::draw2d::text_metric * pMetrics) override;

      virtual bool get_output_text_metrics(::draw2d::text_metric * pMetrics) override;

      //virtual i32 SetTextJustification(i32 nBreakExtra, i32 nBreakCount) override;
      //virtual i32 GetTextCharacterExtra() override;
      //virtual i32 SetTextCharacterExtra(i32 nCharExtra) override;

//      virtual u32 GetCharacterPlacement(string & str, strsize nMaxExtent, LPGCP_RESULTS pResults, u32 dwFlags) override;
//
//      virtual u32 GetCharacterPlacement(const char * pString, strsize nCount, strsize nMaxExtent, LPGCP_RESULTS lpResults, u32 dwFlags) override;
//
//
//#if (_WIN32_WINNT >= 0x0500)
//
//      bool GetTextExtentExPointI(LPWORD pgiIn, i32 cgi, i32 nMaxExtent, LPINT pnFit, LPINT alpDx, __out_opt LPSIZE lpSize) override;
//
//      bool GetTextExtentPointI(LPWORD pgiIn, i32 cgi, __out_opt LPSIZE pSize) override;
//
//
//#endif



      // Advanced Drawing
      //bool DrawEdge(const ::rect & rect,UINT nEdge,UINT nFlags) override;

      //bool DrawFrameControl(const ::rect & rect,UINT nType,UINT nState) override;


//      // Scrolling Functions
//      bool ScrollDC(i32 dx, i32 dy, const rect &  pRectScroll, const rect &  lpRectClip,
//
//                    ::draw2d::region* pRgnUpdate, RECT * pRectUpdate) override;
//
//
//      // font Functions
//      bool GetCharWidth(UINT nFirstChar, UINT nLastChar, LPINT pBuffer) override;
//
//      bool GetOutputCharWidth(UINT nFirstChar, UINT nLastChar, LPINT pBuffer) override;
//
//      u32 SetMapperFlags(u32 dwFlag) override;
//      size GetAspectRatioFilter() override;
//
//      bool GetCharABCWidths(UINT nFirstChar, UINT nLastChar, LPABC pabc) override;
//
//      u32 GetFontData(u32 dwTable, u32 dwOffset, LPVOID pData, u32 cbData) override;
//
//      i32 GetKerningPairs(i32 nPairs, LPKERNINGPAIR pkrnpair) override;
//
//      UINT GetOutlineTextMetrics(UINT cbData, LPOUTLINETEXTMETRICW potm) override;
//
//      u32 GetGlyphOutline(UINT nChar, const ::e_align & ealign = e_align_top_left, const ::e_draw_text & edrawtext = e_draw_text_none, LPGLYPHMETRICS pgm,
//
//                               u32 cbBuffer, LPVOID pBuffer, const MAT2* lpmat2) override;
//
//
//      bool GetCharABCWidths(UINT nFirstChar, UINT nLastChar,
//                            LPABCFLOAT pABCF) override;
//
//      bool GetCharWidth(UINT nFirstChar, UINT nLastChar,
//                        float* pFloatBuffer) override;
//
//
//      u32 GetFontLanguageInfo() override;
//
//#if (_WIN32_WINNT >= 0x0500)
//
//      bool GetCharABCWidthsI(UINT giFirst, UINT cgi, LPWORD pgi, LPABC pabc) override;
//
//      bool GetCharWidthI(UINT giFirst, UINT cgi, LPWORD pgi, LPINT pBuffer) override;
//
//
//#endif
//
//      virtual i32 Escape(i32 nEscape, i32 nCount, const char * pszInData, LPVOID lpOutData) override;
//
//      i32 Escape(i32 nEscape, i32 nInputSize,  const char * pszInputData,i32 nOutputSize, char * pszOutputData) override;
//
//      i32 DrawEscape(i32 nEscape, i32 nInputSize, const char * pszInputData) override;


      // Escape helpers
      i32 StartDoc(const char * pszDocName) override;  // old Win3.0 version

      i32 StartDoc(LPDOCINFOW pDocInfo) override;

      i32 StartPage() override;
      i32 EndPage() override;
      //i32 SetAbortProc(bool (CALLBACK* pfn)(HDC, i32)) override;

      i32 AbortDoc() override;
      i32 EndDoc() override;

      // MetaFile Functions
      bool PlayMetaFile(HMETAFILE hMF) override;
      bool PlayMetaFile(HENHMETAFILE hEnhMetaFile, const rectd &  pBounds) override;

      bool AddMetaFileComment(UINT nDataSize, const BYTE* pCommentData) override;
      // can be used for enhanced metafiles only

      bool abort_path() override;
      bool begin_path() override;
      bool close_figure() override;
      bool end_path() override;
      bool fill_path() override;
      bool flatten_path() override;
      bool stroke_and_fill_path() override;
      bool stroke_path() override;
      bool widen_path() override;

      bool draw_path(::draw2d::path * ppath) override;
      bool draw_path(::draw2d::path * ppath, ::draw2d::pen * ppen) override;
      bool fill_path(::draw2d::path * ppath) override;
      bool fill_path(::draw2d::path * ppath, ::draw2d::brush * pbrush) override;


      float GetMiterLimit() override;
      bool SetMiterLimit(float fMiterLimit) override;
      i32 GetPath(::pointd * ppoint, byte * lpTypes,count nCount) override;

      bool SelectClipPath(i32 nMode) override;

      // Misc helper Functions
      static ::draw2d::brush* GetHalftoneBrush(::layered * pobjectContext);
      //void DrawDragRect(const rect &  prect, const ::size & size,

      //                  const rect &  pRectLast, const ::size & sizeLast,

      //                  ::draw2d::brush* pBrush = nullptr, ::draw2d::brush* pBrushLast = nullptr) override;

      //void fill_rect(const ::rect & rect, const ::color & color) override;

      virtual bool fill_rect(const ::rectd & rect, const ::color & color) override;

      //virtual void draw_3drect(const ::rect & rect, const ::color & colorTopLeft, const ::color& colorBottomRight, const ::e_border & eborder = e_border_all) override;
      virtual bool draw_3drect(const ::rectd & rect, const ::color & colorTopLeft, const ::color& colorBottomRight, const ::e_border & eborder = e_border_all) override;

      //virtual bool draw_rect(const ::rect& rect, const ::color& color, const ::e_border & eborder = e_border_all);
      virtual bool draw_rect(const rectd& rectd, const ::color& color, const ::e_border & eborder = e_border_all);

#ifdef DEBUG
      virtual void assert_valid() const override;
      virtual void dump(dump_context & dumpcontext) const override;
#endif

//      HGDIOBJ set(HGDIOBJ) override;      // do not use for regions

      virtual bool set_alpha_mode(::draw2d::enum_alpha_mode ealphamode) override;

      virtual bool set_smooth_mode(::draw2d::e_smooth_mode esmoothmode) override;

      virtual bool set_interpolation_mode(::draw2d::enum_interpolation_mode einterpolationmode) override;

      virtual bool set_compositing_quality(::draw2d::e_compositing_quality ecompositingquality) override;

      virtual bool set_text_rendering_hint(::draw2d::e_text_rendering_hint etextrendering) override;

      //virtual void * get_os_data() const override;
      //virtual void * get_os_data_ex(int i) const override;
      //virtual void release_os_data_ex(int i, void *point) override;
      virtual HDC get_handle() const;
      //virtual HDC get_handle1() const;
      //virtual HDC get_handle2() const;

//      virtual bool attach(void * pdata) override;


      virtual Gdiplus::FillMode gdiplus_get_fill_mode();

      bool blur(bool bExpand, double dRadius, const ::rectd & rect) override;


      virtual bool flush() override;
      virtual bool sync_flush() override;

      //virtual bool draw_line(double x1, double y1, double x2, double y2) override;
      virtual bool draw_line(double x1, double y1, double x2, double y2, ::draw2d::pen * ppen) override;

      //virtual bool draw_line(const ::pointd & point1, const ::pointd & point2, ::draw2d::pen * ppen) override;


      virtual void enum_fonts(::draw2d::font_enum_item_array & itema) override;

      virtual bool prefer_mapped_image_on_mix() override;

      virtual bool TextOutAlphaBlend(double x, double y, const string & str) override;

      //virtual bool BitBltAlphaBlend(double x, double y, i32 nWidth, i32 nHeight, ::draw2d::graphics * pgraphicsSrc, i32 xSrc, i32 ySrc, u32 dwRop) override;

      virtual i32 meta_file_procedure(HDC hDC, HANDLETABLE* pHandleTable, METARECORD* pMetaRec, i32 nHandles);


   };


} // namespace draw2d_gdiplus



