/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2010-2014 - Hans-Kristian Arntzen
 *  Copyright (C) 2011-2015 - Daniel De Matteis
 * 
 *  RetroArch is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with RetroArch.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../d3d/d3d.h"
#include "../font_d3d_driver.h"
#include "../../general.h"

static LPD3DXFONT d3d_font;

static bool d3dfonts_w32_init_font(void *data,
      const char *font_path, unsigned font_size)
{
   uint32_t r, g, b;
   d3d_video_t *d3d = (d3d_video_t*)data;
   settings_t *settings = config_get_ptr();
   D3DXFONT_DESC desc = {
      static_cast<int>(font_size), 0, 400, 0,
      false, DEFAULT_CHARSET,
      OUT_TT_PRECIS,
      CLIP_DEFAULT_PRECIS,
      DEFAULT_PITCH,
      "Verdana" // Hardcode ftl :(
   };

   (void)font_path;

   r = static_cast<uint32_t>(settings->video.msg_color_r * 255) & 0xff;
   g = static_cast<uint32_t>(settings->video.msg_color_g * 255) & 0xff;
   b = static_cast<uint32_t>(settings->video.msg_color_b * 255) & 0xff;

   d3d->font_color = D3DCOLOR_XRGB(r, g, b);

   return SUCCEEDED(D3DXCreateFontIndirect(d3d->dev, &desc, &d3d_font));
}

static void d3dfonts_w32_free_font(void *data)
{
   (void)data;

   if (d3d_font)
      d3d_font->Release();
   d3d_font = NULL;
}

static void d3dfonts_w32_render_msg(void *data, const char *msg,
      const void *userdata)
{
   d3d_video_t *d3d = (d3d_video_t*)data;
   const struct font_params *params = (const struct font_params*)userdata;

   if (!d3d)
      return;
   if (!msg)
      return;
   if (!(SUCCEEDED(d3d->dev->BeginScene())))
      return;

   d3d_font->DrawTextA(NULL,
         msg,
         -1,
         &d3d->font_rect_shifted,
         DT_LEFT,
         ((d3d->font_color >> 2) & 0x3f3f3f) | 0xff000000);

   d3d_font->DrawTextA(NULL,
         msg,
         -1,
         &d3d->font_rect,
         DT_LEFT,
         d3d->font_color | 0xff000000);

   d3d->dev->EndScene();
}

d3d_font_renderer_t d3d_win32_font = {
   d3dfonts_w32_init_font,
   d3dfonts_w32_free_font,
   d3dfonts_w32_render_msg,
   "d3d-fonts-w32",
   NULL,
   NULL,
   NULL,
};
