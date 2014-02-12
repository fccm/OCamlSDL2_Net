/* OCamlSDL2_Net - An OCaml interface to the SDL2_Net library
 Copyright (C) 2013 Florent Monnier
 
 This software is provided "AS-IS", without any express or implied warranty.
 In no event will the authors be held liable for any damages arising from
 the use of this software.
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it freely.
*/
#define CAML_NAME_SPACE
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/fail.h>

#include <SDL_net.h>

CAMLprim value
caml_SDLNet_Init(value unit)
{
    int r = SDLNet_Init();
    if (r) caml_failwith("Sdlnet.init");
    return Val_unit;
}

CAMLprim value
caml_SDLNet_Quit(value unit)
{
    SDLNet_Quit();
    return Val_unit;
}

CAMLprim value
caml_SDLNet_Linked_Version(value unit)
{
  CAMLparam1(unit);
  CAMLlocal1(ver);

  const SDL_version *link_version = SDLNet_Linked_Version();

  ver = caml_alloc(3, 0);
  Store_field(ver, 0, Val_int(link_version->major));
  Store_field(ver, 1, Val_int(link_version->minor));
  Store_field(ver, 2, Val_int(link_version->patch));
  CAMLreturn(ver);
}

/* vim: set ts=4 sw=4 et: */
