/*
 * gVirtuS -- A GPGPU transparent virtualization component.
 *
 * Copyright (C) 2009-2011  The University of Napoli Parthenope at Naples.
 *
 * This file is part of gVirtuS.
 *
 * gVirtuS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * gVirtuS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gVirtuS; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Written by: Giuseppe Coviello <giuseppe.coviello@uniparthenope.it>,
 *             Department of Applied Science
 */

#include <GL/glx.h>

#include "GLHandler.h"

using namespace std;

GL_ROUTINE_HANDLER(XChooseFBConfig) {
    Display *dpy = GetDisplay();
    int screen = in->Get<int>();
    int *attrib_list = in->AssignAll<int>();
    int nelements;
    GLXFBConfig *config = glXChooseFBConfig(dpy, screen, attrib_list,
            &nelements);
    Buffer *out = new Buffer();
    out->Add(nelements);
    for(int i = 0; i < nelements; i++)
        out->Add((uint64_t) config[i]);
    return new Result(0, out);
}

GL_ROUTINE_HANDLER(XChooseVisual) {
    //dpy = XOpenDisplay(in->AssignString());
    in->AssignString();
    in->Get<int>();
    in->AssignAll<int>();
    //info = glXChooseVisual(dpy, screen, attribList);
    //cout << attribList << endl;
    //cout << screen << endl;
    //for(int i = 0; attribList[i] != None; i++)
    //    cout << attribList[i] << endl;
    //if(info == NULL)
    //    return new Result(-1);
    //cout << XVisualIDFromVisual(info->visual) << endl;
    XVisualInfo *info = GetVisualInfo();
    Buffer *out = new Buffer();
    out->Add(info->visual);
    out->Add(info);
    out->Add(XVisualIDFromVisual(info->visual));
    return new Result(0, out);
}

GL_ROUTINE_HANDLER(XCreateContext) {
    GLXContext shareList = (GLXContext) in->Get<uint64_t > ();
    Bool direct = in->Get<Bool > ();
    GLXContext ctx = glXCreateContext(GetDisplay(), GetVisualInfo(), shareList, direct);
    cout << "Created GLX Context: " << ctx << endl;
    if (ctx == NULL)
        return new Result(-1);
    Buffer *out = new Buffer();
    out->Add((uint64_t) ctx);
    return new Result(0, out);
}

GL_ROUTINE_HANDLER(XGetClientString) {
    int name = in->Get<int>();
    Buffer *out = new Buffer();
    out->AddString(glXGetClientString(GetDisplay(), name));
    return new Result(0, out);
}

GL_ROUTINE_HANDLER(XMakeContextCurrent) {
    GLXDrawable draw = GetDrawable(in->Get<GLXDrawable > (), pThis);
    GLXDrawable read = GetDrawable(in->Get<GLXDrawable > (), pThis);
    GLXContext ctx = (GLXContext) in->Get<uint64_t > ();
    Bool result = glXMakeContextCurrent(GetDisplay(), draw, read, ctx);
    Buffer *out = new Buffer();
    out->Add(result);
    return new Result(0, out);
}

GL_ROUTINE_HANDLER(XMakeCurrent) {
    GLXDrawable drawable = GetDrawable(in->Get<GLXDrawable>(), pThis);
    GLXContext ctx = (GLXContext) in->Get<uint64_t > ();
    cout << "ctx: " << ctx << endl;
    /*bool use_shm = */ in->Get<bool>();
    Bool result = glXMakeCurrent(GetDisplay(), drawable, ctx);
    Buffer *out = new Buffer();
    out->Add(result);
    return new Result((result == false ? -1 : 0), out);
}

GL_ROUTINE_HANDLER(XQueryExtension) {
    int errorBase;
    int eventBase;
    glXQueryExtension(GetDisplay(), &errorBase, &eventBase);
    Buffer *out = new Buffer();
    out->Add(errorBase);
    out->Add(eventBase);
    return new Result(0, out);
}

GL_ROUTINE_HANDLER(XQueryExtensionsString) {
    /*int screen =*/ in->Get<int>();
    const char *result = ""; //glXQueryExtensionsString(GetDisplay(), screen);
    Buffer *out = new Buffer();
    out->AddString(result);
    return new Result(0, out);
}

GL_ROUTINE_HANDLER(XSwapBuffers) {
    GLXDrawable drawable = GetDrawable(in->Get<GLXDrawable>(), pThis);
    glXSwapBuffers(GetDisplay(), drawable);
    pThis->Lock();
    char *buffer = pThis->GetFramebuffer();
    glReadPixels(0, 0, 512, 512, GL_BGRA, GL_UNSIGNED_BYTE, buffer);
    pThis->Unlock();
    return new Result(0);
}