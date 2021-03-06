#pragma once


namespace draw2d_gdiplus
{


   class region :
      virtual public ::draw2d::region
   {
   public:


      Gdiplus::Region *       m_pregion;


      region();
      virtual ~region();


      virtual bool create(::draw2d::graphics * pgraphics, ::i8 iCreate) override;
      virtual void destroy() override;


      virtual bool translate(const POINT_I32 & point, ::draw2d::graphics * pgraphics = nullptr) override;

      virtual bool contains(const POINT_I32 & point, ::draw2d::graphics * pgraphics = nullptr) override;

      virtual bool get_bounding_box(RECTANGLE_I32 * prectangle, ::draw2d::graphics * pgraphics = nullptr) override;

      virtual void max_bounding_box(RECTANGLE_F64 * prectangle, ::draw2d::graphics * pgraphics) override;

      Gdiplus::Region * get(::draw2d::graphics * pgraphics);
      Gdiplus::Region * get_rectangle(::draw2d::graphics * pgraphics);
      Gdiplus::Region * get_ellipse(::draw2d::graphics * pgraphics);
      Gdiplus::Region * get_polygon(::draw2d::graphics * pgraphics);
      Gdiplus::Region * get_poly_polygon(::draw2d::graphics * pgraphics);
      Gdiplus::Region * get_combine(::draw2d::graphics * pgraphics);


   };


} // namespace draw2d_gdiplus



