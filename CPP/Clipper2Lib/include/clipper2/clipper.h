/*******************************************************************************
* Author    :  Angus Johnson                                                   *
* Date      :  23 January 2023                                                 *
* Website   :  http://www.angusj.com                                           *
* Copyright :  Angus Johnson 2010-2023                                         *
* Purpose   :  This module provides a simple interface to the Clipper Library  *
* License   :  http://www.boost.org/LICENSE_1_0.txt                            *
*******************************************************************************/

#ifndef CLIPPER_H
#define CLIPPER_H

#include <cstdlib>
#include <vector>

#include "clipper.core.h"
#include "clipper.engine.h"
#include "clipper.offset.h"
#include "clipper.minkowski.h"
#include "clipper.rectclip.h"

namespace Clipper2Lib {

  inline Paths64 BooleanOp(ClipType cliptype, FillRule fillrule,
    const Paths64& subjects, const Paths64& clips)
  {    
    Paths64 result;
    Clipper64 clipper;
    clipper.AddSubject(subjects);
    clipper.AddClip(clips);
    clipper.Execute(cliptype, fillrule, result);
    return result;
  }

  inline void BooleanOp(ClipType cliptype, FillRule fillrule,
    const Paths64& subjects, const Paths64& clips, PolyTree64& solution)
  {
    Paths64 sol_open;
    Clipper64 clipper;
    clipper.AddSubject(subjects);
    clipper.AddClip(clips);
    clipper.Execute(cliptype, fillrule, solution, sol_open);
  }

  inline PathsD BooleanOp(ClipType cliptype, FillRule fillrule,
    const PathsD& subjects, const PathsD& clips, int decimal_prec = 2)
  {
    CheckPrecision(decimal_prec);
    PathsD result;
    ClipperD clipper(decimal_prec);
    clipper.AddSubject(subjects);
    clipper.AddClip(clips);
    clipper.Execute(cliptype, fillrule, result);
    return result;
  }

  inline void BooleanOp(ClipType cliptype, FillRule fillrule,
    const PathsD& subjects, const PathsD& clips, 
    PolyTreeD& polytree, int decimal_prec = 2)
  {
    CheckPrecision(decimal_prec);
    PathsD result;
    ClipperD clipper(decimal_prec);
    clipper.AddSubject(subjects);
    clipper.AddClip(clips);
    clipper.Execute(cliptype, fillrule, polytree);
  }

  inline Paths64 Intersect(const Paths64& subjects, const Paths64& clips, FillRule fillrule)
  {
    return BooleanOp(ClipType::Intersection, fillrule, subjects, clips);
  }
  
  inline PathsD Intersect(const PathsD& subjects, const PathsD& clips, FillRule fillrule, int decimal_prec = 2)
  {
    return BooleanOp(ClipType::Intersection, fillrule, subjects, clips, decimal_prec);
  }

  inline Paths64 Union(const Paths64& subjects, const Paths64& clips, FillRule fillrule)
  {
    return BooleanOp(ClipType::Union, fillrule, subjects, clips);
  }

  inline PathsD Union(const PathsD& subjects, const PathsD& clips, FillRule fillrule, int decimal_prec = 2)
  {
    return BooleanOp(ClipType::Union, fillrule, subjects, clips, decimal_prec);
  }

  inline Paths64 Union(const Paths64& subjects, FillRule fillrule)
  {
    Paths64 result;
    Clipper64 clipper;
    clipper.AddSubject(subjects);
    clipper.Execute(ClipType::Union, fillrule, result);
    return result;
  }

  inline PathsD Union(const PathsD& subjects, FillRule fillrule, int decimal_prec = 2)
  {
    CheckPrecision(decimal_prec);
    PathsD result;
    ClipperD clipper(decimal_prec);
    clipper.AddSubject(subjects);
    clipper.Execute(ClipType::Union, fillrule, result);
    return result;
  }

  inline Paths64 Difference(const Paths64& subjects, const Paths64& clips, FillRule fillrule)
  {
    return BooleanOp(ClipType::Difference, fillrule, subjects, clips);
  }

  inline PathsD Difference(const PathsD& subjects, const PathsD& clips, FillRule fillrule, int decimal_prec = 2)
  {
    return BooleanOp(ClipType::Difference, fillrule, subjects, clips, decimal_prec);
  }

  inline Paths64 Xor(const Paths64& subjects, const Paths64& clips, FillRule fillrule)
  {
    return BooleanOp(ClipType::Xor, fillrule, subjects, clips);
  }

  inline PathsD Xor(const PathsD& subjects, const PathsD& clips, FillRule fillrule, int decimal_prec = 2)
  {
    return BooleanOp(ClipType::Xor, fillrule, subjects, clips, decimal_prec);
  }

  inline Paths64 InflatePaths(const Paths64& paths, double delta,
    JoinType jt, EndType et, double miter_limit = 2.0)
  {
    ClipperOffset clip_offset(miter_limit);
    clip_offset.AddPaths(paths, jt, et);
    return clip_offset.Execute(delta);
  }

  inline PathsD InflatePaths(const PathsD& paths, double delta,
    JoinType jt, EndType et, double miter_limit = 2.0, int precision = 2)
  {
    CheckPrecision(precision);
    const double scale = std::pow(10, precision);
    ClipperOffset clip_offset(miter_limit);
    clip_offset.AddPaths(ScalePaths<int64_t,double>(paths, scale), jt, et);
    Paths64 tmp = clip_offset.Execute(delta * scale);
    return ScalePaths<double, int64_t>(tmp, 1 / scale);
  }

  inline Path64 TranslatePath(const Path64& path, int64_t dx, int64_t dy)
  {
    Path64 result;
    result.reserve(path.size());
    std::transform(path.begin(), path.end(), back_inserter(result),
      [dx, dy](const auto& pt) { return Point64(pt.x + dx, pt.y +dy); });
    return result;
  }

  inline PathD TranslatePath(const PathD& path, double dx, double dy)
  {
    PathD result;
    result.reserve(path.size());
    std::transform(path.begin(), path.end(), back_inserter(result),
      [dx, dy](const auto& pt) { return PointD(pt.x + dx, pt.y + dy); });
    return result;
  }

  inline Paths64 TranslatePaths(const Paths64& paths, int64_t dx, int64_t dy)
  {
    Paths64 result;
    result.reserve(paths.size());
    std::transform(paths.begin(), paths.end(), back_inserter(result),
      [dx, dy](const auto& path) { return TranslatePath(path, dx, dy); });
    return result;
  }

  inline PathsD TranslatePaths(const PathsD& paths, double dx, double dy)
  {
    PathsD result;
    result.reserve(paths.size());
    std::transform(paths.begin(), paths.end(), back_inserter(result),
      [dx, dy](const auto& path) { return TranslatePath(path, dx, dy); });
    return result;
  }

  inline Path64 RectClip(const Rect64& rect, const Path64& path)
  {
    if (rect.IsEmpty() || path.empty()) return Path64();
    Rect64 pathRec = Bounds(path);
    if (!rect.Intersects(pathRec)) return Path64();
    if (rect.Contains(pathRec)) return path;
    class RectClip rc(rect);
    return rc.Execute(path);
  }
  
  inline Paths64 RectClip(const Rect64& rect, const Paths64& paths)
  {
    if (rect.IsEmpty() || paths.empty()) return Paths64();
    class RectClip rc(rect);
    Paths64 result;
    result.reserve(paths.size());

    for (const Path64& p : paths)
    {
      Rect64 pathRec = Bounds(p);
      if (!rect.Intersects(pathRec)) 
        continue;
      else if (rect.Contains(pathRec))
        result.push_back(p);
      else
      {
        Path64 p2 = rc.Execute(p);
        if (!p2.empty()) result.push_back(std::move(p2));
      }
    }
    return result;
  }

  inline PathD RectClip(const RectD& rect, const PathD& path, int precision = 2)
  {
    if (rect.IsEmpty() || path.empty() ||
      !rect.Contains(Bounds(path))) return PathD();
    CheckPrecision(precision);
    const double scale = std::pow(10, precision);
    Rect64 r = ScaleRect<int64_t, double>(rect, scale);
    class RectClip rc(r);
    Path64 p = ScalePath<int64_t, double>(path, scale);
    return ScalePath<double, int64_t>(rc.Execute(p), 1 / scale);
  }

  inline PathsD RectClip(const RectD& rect, const PathsD& paths, int precision = 2)
  {
    if (rect.IsEmpty() || paths.empty()) return PathsD();
    CheckPrecision(precision);
    const double scale = std::pow(10, precision);
    Rect64 r = ScaleRect<int64_t, double>(rect, scale);
    class RectClip rc(r);
    PathsD result;
    result.reserve(paths.size());
    for (const PathD& path : paths) 
    {
      RectD pathRec = Bounds(path);
      if (!rect.Intersects(pathRec))
        continue;
      else if (rect.Contains(pathRec))
        result.push_back(path);
      else
      {
        Path64 p = ScalePath<int64_t, double>(path, scale);
        p = rc.Execute(p);
        if (!p.empty()) 
          result.push_back(ScalePath<double, int64_t>(p, 1 / scale));
      }
    }
    return result;
  }

  inline Paths64 RectClipLines(const Rect64& rect, const Path64& path)
  {
    Paths64 result;
    if (rect.IsEmpty() || path.empty()) return result;
    Rect64 pathRec = Bounds(path);
    if (!rect.Intersects(pathRec)) return result;
    if (rect.Contains(pathRec)) 
    {
      result.push_back(path);
      return result;
    }
    class RectClipLines rcl(rect);
    return rcl.Execute(path);
  }

  inline Paths64 RectClipLines(const Rect64& rect, const Paths64& paths)
  {
    Paths64 result;
    if (rect.IsEmpty() || paths.empty()) return result;
    class RectClipLines rcl(rect);
    for (const Path64& p : paths)
    {
      Rect64 pathRec = Bounds(p);
      if (!rect.Intersects(pathRec))
        continue;
      else if (rect.Contains(pathRec))
        result.push_back(p);
      else
      {
        Paths64 pp = rcl.Execute(p);
        if (!pp.empty()) 
          result.insert(result.end(), pp.begin(), pp.end());
      }
    }
    return result;
  }

  inline PathsD RectClipLines(const RectD& rect, const PathD& path, int precision = 2)
  {
    if (rect.IsEmpty() || path.empty() ||
      !rect.Contains(Bounds(path))) return PathsD();
    CheckPrecision(precision);
    const double scale = std::pow(10, precision);
    Rect64 r = ScaleRect<int64_t, double>(rect, scale);
    class RectClipLines rcl(r);
    Path64 p = ScalePath<int64_t, double>(path, scale);
    return ScalePaths<double, int64_t>(rcl.Execute(p), 1 / scale);
  }

  inline PathsD RectClipLines(const RectD& rect, const PathsD& paths, int precision = 2)
  {
    PathsD result;
    if (rect.IsEmpty() || paths.empty()) return result;
    CheckPrecision(precision);
    const double scale = std::pow(10, precision);
    Rect64 r = ScaleRect<int64_t, double>(rect, scale);
    class RectClipLines rcl(r);
    result.reserve(paths.size());
    for (const PathD& path : paths)
    {
      RectD pathRec = Bounds(path);
      if (!rect.Intersects(pathRec))
        continue;
      else if (rect.Contains(pathRec))
        result.push_back(path);
      else
      {
        Path64 p = ScalePath<int64_t, double>(path, scale);
        Paths64 pp = rcl.Execute(p);
        if (pp.empty()) continue;
        PathsD ppd = ScalePaths<double, int64_t>(pp, 1 / scale);
        result.insert(result.end(), ppd.begin(), ppd.end());
      }
    }
    return result;
  }

  namespace details
  {

    inline void PolyPathToPaths64(const PolyPath64& polypath, Paths64& paths)
    {
      paths.push_back(polypath.Polygon());
      for (const auto& child : polypath)
        PolyPathToPaths64(*child, paths);
    }

    inline void PolyPathToPathsD(const PolyPathD& polypath, PathsD& paths)
    {
      paths.push_back(polypath.Polygon());
      for (const auto& child : polypath)
        PolyPathToPathsD(*child, paths);
    }

    inline bool PolyPath64ContainsChildren(const PolyPath64& pp)
    {
      for (const auto& child : pp)
      {
        // return false if this child isn't fully contained by its parent

        // the following algorithm is a bit too crude, and doesn't account
        // for rounding errors. A better algorithm is to return false when
        // consecutive vertices are found outside the parent's polygon.

        //const Path64& path = pp.Polygon();
        //if (std::any_of(child->Polygon().cbegin(), child->Polygon().cend(),
        //  [path](const auto& pt) {return (PointInPolygon(pt, path) ==
        //    PointInPolygonResult::IsOutside); })) return false;

        int outsideCnt = 0;
        for (const Point64& pt : child->Polygon())
        {
          PointInPolygonResult result = PointInPolygon(pt, pp.Polygon());
          if (result == PointInPolygonResult::IsInside) --outsideCnt;
          else if (result == PointInPolygonResult::IsOutside) ++outsideCnt;
          if (outsideCnt > 1) return false;
          else if (outsideCnt < -1) break;
        }

        // now check any nested children too
        if (child->Count() > 0 && !PolyPath64ContainsChildren(*child))
          return false;
      }
      return true;
    }

    inline bool GetInt(std::string::const_iterator& iter, const
      std::string::const_iterator& end_iter, int64_t& val)
    {
      val = 0;
      bool is_neg = *iter == '-';
      if (is_neg) ++iter;
      std::string::const_iterator start_iter = iter;
      while (iter != end_iter &&
        ((*iter >= '0') && (*iter <= '9')))
      {
        val = val * 10 + (static_cast<int64_t>(*iter++) - '0');
      }
      if (is_neg) val = -val;
      return (iter != start_iter);
    }

    inline bool GetFloat(std::string::const_iterator& iter, const 
      std::string::const_iterator& end_iter, double& val)
    {
      val = 0;
      bool is_neg = *iter == '-';
      if (is_neg) ++iter;
      int dec_pos = 1;
      const std::string::const_iterator start_iter = iter;
      while (iter != end_iter && (*iter == '.' ||
        ((*iter >= '0') && (*iter <= '9'))))
      {
        if (*iter == '.')
        {
          if (dec_pos != 1) break;
          dec_pos = 0;
          ++iter;
          continue;
        }
        if (dec_pos != 1) --dec_pos;
        val = val * 10 + ((int64_t)(*iter++) - '0');
      }
      if (iter == start_iter || dec_pos == 0) return false;
      if (dec_pos < 0)
        val *= std::pow(10, dec_pos);
      if (is_neg)
        val *= -1;
      return true;
    }

    inline void SkipWhiteSpace(std::string::const_iterator& iter, 
      const std::string::const_iterator& end_iter)
    {
      while (iter != end_iter && *iter <= ' ') ++iter;
    }

    inline void SkipSpacesWithOptionalComma(std::string::const_iterator& iter, 
      const std::string::const_iterator& end_iter)
    {
      bool comma_seen = false;
      while (iter != end_iter)
      {
        if (*iter == ' ') ++iter;
        else if (*iter == ',')
        {
          if (comma_seen) return; // don't skip 2 commas!
          comma_seen = true;
          ++iter;
        }
        else return;                
      }
    }

    inline bool has_one_match(const char c, char* chrs)
    {
      while (*chrs > 0 && c != *chrs) ++chrs;
      if (!*chrs) return false;
      *chrs = ' '; // only match once per char
      return true;
    }


    inline void SkipUserDefinedChars(std::string::const_iterator& iter,
      const std::string::const_iterator& end_iter, const std::string& skip_chars)
    {
      const size_t MAX_CHARS = 16;
      char buff[MAX_CHARS] = {0};
      std::copy(skip_chars.cbegin(), skip_chars.cend(), &buff[0]);
      while (iter != end_iter && 
        (*iter <= ' ' || has_one_match(*iter, buff))) ++iter;
      return;
    }

    static void OutlinePolyPath(std::ostream& os, 
      bool isHole, size_t count, const std::string& preamble)
    {
      std::string plural = (count == 1) ? "." : "s.";
      if (isHole)
      {
        if (count)
          os << preamble << "+- Hole with " << count <<
          " nested polygon" << plural << std::endl;
        else
          os << preamble << "+- Hole" << std::endl;
      }
      else
      {
        if (count)
          os << preamble << "+- Polygon with " << count <<
          " hole" << plural << std::endl;
        else
          os << preamble << "+- Polygon" << std::endl;
      }
    }

    static void OutlinePolyPath64(std::ostream& os, const PolyPath64& pp,
      std::string preamble, bool last_child)
    {
      OutlinePolyPath(os, pp.IsHole(), pp.Count(), preamble);
      preamble += (!last_child) ? "|  " : "   ";
      if (pp.Count())
      {
        PolyPath64List::const_iterator it = pp.begin();
        for (; it < pp.end() - 1; ++it)
          OutlinePolyPath64(os, **it, preamble, false);
        OutlinePolyPath64(os, **it, preamble, true);
      }
    }

    static void OutlinePolyPathD(std::ostream& os, const PolyPathD& pp,
      std::string preamble, bool last_child)
    {
      OutlinePolyPath(os, pp.IsHole(), pp.Count(), preamble);
      preamble += (!last_child) ? "|  " : "   ";
      if (pp.Count())
      {
        PolyPathDList::const_iterator it = pp.begin();
        for (; it < pp.end() - 1; ++it)
          OutlinePolyPathD(os, **it, preamble, false);
        OutlinePolyPathD(os, **it, preamble, true);
      }
    }

  } // end details namespace 

  inline std::ostream& operator<< (std::ostream& os, const PolyTree64& pp)
  {
    PolyPath64List::const_iterator it = pp.begin();
    for (; it < pp.end() - 1; ++it)
      details::OutlinePolyPath64(os, **it, "   ", false);
    details::OutlinePolyPath64(os, **it, "   ", true);
    os << std::endl << std::endl;
    if (!pp.Level()) os << std::endl;
    return os;
  }

  inline std::ostream& operator<< (std::ostream& os, const PolyTreeD& pp)
  {
    PolyPathDList::const_iterator it = pp.begin();
    for (; it < pp.end() - 1; ++it)
      details::OutlinePolyPathD(os, **it, "   ", false);
    details::OutlinePolyPathD(os, **it, "   ", true);
    os << std::endl << std::endl;
    if (!pp.Level()) os << std::endl;
    return os;
  }

  inline Paths64 PolyTreeToPaths64(const PolyTree64& polytree)
  {
    Paths64 result;
    for (const auto& child : polytree)
      details::PolyPathToPaths64(*child, result);
    return result;
  }

  inline PathsD PolyTreeToPathsD(const PolyTreeD& polytree)
  {
    PathsD result;
    for (const auto& child : polytree)
      details::PolyPathToPathsD(*child, result);
    return result;
  }

  inline bool CheckPolytreeFullyContainsChildren(const PolyTree64& polytree)
  {
    for (const auto& child : polytree)
      if (child->Count() > 0 && 
        !details::PolyPath64ContainsChildren(*child))
          return false;
    return true;
  }

  inline Path64 MakePath(const std::string& s)
  {
    const std::string skip_chars = " ,(){}[]";
    Path64 result;
    std::string::const_iterator s_iter = s.cbegin();
    details::SkipUserDefinedChars(s_iter, s.cend(), skip_chars);
    while (s_iter != s.cend())
    {
      int64_t y = 0, x = 0;
      if (!details::GetInt(s_iter, s.cend(), x)) break;
      details::SkipSpacesWithOptionalComma(s_iter, s.cend());
      if (!details::GetInt(s_iter, s.cend(), y)) break;
      result.push_back(Point64(x, y));
      details::SkipUserDefinedChars(s_iter, s.cend(), skip_chars);
    }
    return result;
  }
  
  inline PathD MakePathD(const std::string& s)
  {
    const std::string skip_chars = " ,(){}[]";
    PathD result;
    std::string::const_iterator s_iter = s.cbegin();
    details::SkipUserDefinedChars(s_iter, s.cend(), skip_chars);
    while (s_iter != s.cend())
    {
      double y = 0, x = 0;
      if (!details::GetFloat(s_iter, s.cend(), x)) break;
      details::SkipSpacesWithOptionalComma(s_iter, s.cend());
      if (!details::GetFloat(s_iter, s.cend(), y)) break;
      result.push_back(PointD(x, y));
      details::SkipUserDefinedChars(s_iter, s.cend(), skip_chars);
    }
    return result;
  }

  inline Path64 TrimCollinear(const Path64& p, bool is_open_path = false)
  {
    size_t len = p.size();
    if (len < 3)
    {
      if (!is_open_path || len < 2 || p[0] == p[1]) return Path64();
      else return p;
    }

    Path64 dst;
    dst.reserve(len);
    Path64::const_iterator srcIt = p.cbegin(), prevIt, stop = p.cend() - 1;

    if (!is_open_path)
    {
      while (srcIt != stop && !CrossProduct(*stop, *srcIt, *(srcIt + 1)))
        ++srcIt;
      while (srcIt != stop && !CrossProduct(*(stop - 1), *stop, *srcIt))
        --stop;
      if (srcIt == stop) return Path64();
    }

    prevIt = srcIt++;
    dst.push_back(*prevIt);
    for (; srcIt != stop; ++srcIt)
    {
      if (CrossProduct(*prevIt, *srcIt, *(srcIt + 1)))
      {
        prevIt = srcIt;
        dst.push_back(*prevIt);
      }
    }

    if (is_open_path)
      dst.push_back(*srcIt);
    else if (CrossProduct(*prevIt, *stop, dst[0]))
      dst.push_back(*stop);
    else
    {
      while (dst.size() > 2 &&
        !CrossProduct(dst[dst.size() - 1], dst[dst.size() - 2], dst[0]))
          dst.pop_back();
      if (dst.size() < 3) return Path64();
    }
    return dst;
  }

  inline PathD TrimCollinear(const PathD& path, int precision, bool is_open_path = false)
  {
    CheckPrecision(precision);
    const double scale = std::pow(10, precision);
    Path64 p = ScalePath<int64_t, double>(path, scale);
    p = TrimCollinear(p, is_open_path);
    return ScalePath<double, int64_t>(p, 1/scale);
  }

  template <typename T>
  inline double Distance(const Point<T> pt1, const Point<T> pt2)
  {
    return std::sqrt(DistanceSqr(pt1, pt2));
  }

  template <typename T>
  inline double Length(const Path<T>& path, bool is_closed_path = false)
  {
    double result = 0.0;
    if (path.size() < 2) return result;
    auto it = path.cbegin(), stop = path.end() - 1;
    for (; it != stop; ++it)
      result += Distance(*it, *(it + 1));
    if (is_closed_path)
      result += Distance(*stop, *path.cbegin());
    return result;
  }


  template <typename T>
  inline bool NearCollinear(const Point<T>& pt1, const Point<T>& pt2, const Point<T>& pt3, double sin_sqrd_min_angle_rads)
  {
    double cp = std::abs(CrossProduct(pt1, pt2, pt3));
    return (cp * cp) / (DistanceSqr(pt1, pt2) * DistanceSqr(pt2, pt3)) < sin_sqrd_min_angle_rads;
  }
  
  template <typename T>
  inline Path<T> Ellipse(const Rect<T>& rect, int steps = 0)
  {
    return Ellipse(rect.MidPoint(), 
      static_cast<double>(rect.Width()) *0.5, 
      static_cast<double>(rect.Height()) * 0.5, steps);
  }

  template <typename T>
  inline Path<T> Ellipse(const Point<T>& center,
    double radiusX, double radiusY = 0, int steps = 0)
  {
    if (radiusX <= 0) return Path<T>();
    if (radiusY <= 0) radiusY = radiusX;
    if (steps <= 2)
      steps = static_cast<int>(PI * sqrt((radiusX + radiusY) / 2));

    double si = std::sin(2 * PI / steps);
    double co = std::cos(2 * PI / steps);
    double dx = co, dy = si;
    Path<T> result;
    result.reserve(steps);
    result.push_back(Point<T>(center.x + radiusX, static_cast<double>(center.y)));
    for (int i = 1; i < steps; ++i)
    {
      result.push_back(Point<T>(center.x + radiusX * dx, center.y + radiusY * dy));
      double x = dx * co - dy * si;
      dy = dy * co + dx * si;
      dx = x;
    }
    return result;
  }

  template <typename T>
  inline double PerpendicDistFromLineSqrd(const Point<T>& pt,
    const Point<T>& line1, const Point<T>& line2)
  {
    double a = static_cast<double>(pt.x - line1.x);
    double b = static_cast<double>(pt.y - line1.y);
    double c = static_cast<double>(line2.x - line1.x);
    double d = static_cast<double>(line2.y - line1.y);
    if (c == 0 && d == 0) return 0;
    return Sqr(a * d - c * b) / (c * c + d * d);
  }

  inline size_t GetNext(size_t current, size_t high, 
    const std::vector<bool>& flags)
  {
    ++current;
    while (current <= high && flags[current]) ++current;
    if (current <= high) return current;
    current = 0;
    while (flags[current]) ++current;
    return current;
  }

  inline size_t GetPrior(size_t current, size_t high, 
    const std::vector<bool>& flags)
  {
    if (current == 0) current = high;
    else --current;
    while (current > 0 && flags[current]) --current;
    if (!flags[current]) return current;
    current = high;
    while (flags[current]) --current;
    return current;
  }

  template <typename T>
  inline Path<T> SimplifyPath(const Path<T> path, 
    double epsilon, bool isOpenPath = false)
  {
    const size_t len = path.size(), high = len -1;
    const double epsSqr = Sqr(epsilon);
    if (len < 4) return Path<T>(path);

    std::vector<bool> flags(len);
    std::vector<double> distSqr(len);
    size_t prior = high, curr = 0, start, next, prior2, next2;
    if (isOpenPath)
    {
      distSqr[0] = DBL_MAX;
      distSqr[high] = DBL_MAX;
    }
    else 
    {
      distSqr[0] = PerpendicDistFromLineSqrd(path[0], path[high], path[1]);
      distSqr[high] = PerpendicDistFromLineSqrd(path[high], path[0], path[high - 1]);
    }
    for (size_t i = 1; i < high; ++i)
      distSqr[i] = PerpendicDistFromLineSqrd(path[i], path[i - 1], path[i + 1]);

    for (;;)
    {
      if (distSqr[curr] > epsSqr)
      {
        start = curr;
        do
        {
          curr = GetNext(curr, high, flags);
        } while (curr != start && distSqr[curr] > epsSqr);
        if (curr == start) break;
      }

      prior = GetPrior(curr, high, flags);
      next = GetNext(curr, high, flags);
      if (next == prior) break;

      if (distSqr[next] < distSqr[curr])
      {
        flags[next] = true;
        next = GetNext(next, high, flags);
        next2 = GetNext(next, high, flags);
        distSqr[curr] = PerpendicDistFromLineSqrd(path[curr], path[prior], path[next]);
        if (next != high || !isOpenPath)
          distSqr[next] = PerpendicDistFromLineSqrd(path[next], path[curr], path[next2]);
        curr = next;
      }
      else
      {
        flags[curr] = true;
        curr = next;
        next = GetNext(next, high, flags);
        prior2 = GetPrior(prior, high, flags);
        distSqr[curr] = PerpendicDistFromLineSqrd(path[curr], path[prior], path[next]);
        if (prior != 0 || !isOpenPath)
          distSqr[prior] = PerpendicDistFromLineSqrd(path[prior], path[prior2], path[curr]);
      }
    }
    Path<T> result;
    result.reserve(len);
    for (typename Path<T>::size_type i = 0; i < len; ++i)
      if (!flags[i]) result.push_back(path[i]);
    return result;
  }

  template <typename T>
  inline Paths<T> SimplifyPaths(const Paths<T> paths, 
    double epsilon, bool isOpenPath = false)
  {
    Paths<T> result;
    result.reserve(paths.size());
    for (const auto& path : paths)
      result.push_back(SimplifyPath(path, epsilon, isOpenPath));
    return result;
  }


  template <typename T>
  inline void RDP(const Path<T> path, std::size_t begin,
    std::size_t end, double epsSqrd, std::vector<bool>& flags)
  {
    typename Path<T>::size_type idx = 0;
    double max_d = 0;
    while (end > begin && path[begin] == path[end]) flags[end--] = false;
    for (typename Path<T>::size_type i = begin + 1; i < end; ++i)
    {
      // PerpendicDistFromLineSqrd - avoids expensive Sqrt()
      double d = PerpendicDistFromLineSqrd(path[i], path[begin], path[end]);
      if (d <= max_d) continue;
      max_d = d;
      idx = i;
    }
    if (max_d <= epsSqrd) return;
    flags[idx] = true;
    if (idx > begin + 1) RDP(path, begin, idx, epsSqrd, flags);
    if (idx < end - 1) RDP(path, idx, end, epsSqrd, flags);
  }

  template <typename T>
  inline Path<T> RamerDouglasPeucker(const Path<T>& path, double epsilon)
  {
    const typename Path<T>::size_type len = path.size();
    if (len < 5) return Path<T>(path);
    std::vector<bool> flags(len);
    flags[0] = true;
    flags[len - 1] = true;
    RDP(path, 0, len - 1, Sqr(epsilon), flags);
    Path<T> result;
    result.reserve(len);
    for (typename Path<T>::size_type i = 0; i < len; ++i)
      if (flags[i])
        result.push_back(path[i]);
    return result;
  }

  template <typename T>
  inline Paths<T> RamerDouglasPeucker(const Paths<T>& paths, double epsilon)
  {
    Paths<T> result;
    result.reserve(paths.size());
    std::transform(paths.begin(), paths.end(), back_inserter(result),
      [epsilon](const auto& path)
      { return RamerDouglasPeucker<T>(path, epsilon); });
    return result;
  }



}  // end Clipper2Lib namespace

#endif  // CLIPPER_H
