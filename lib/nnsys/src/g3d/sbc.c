#include "nnsys/g3d.h"

// NitroSystem's G3D scene binary commands: the drawing of a render object, NNS_G3DDraw and the render functions it
// dispatches to. The file is named after the SDK's own G3D object, as pret/pokediamond's linker script lists it.

static void func_020662bc(u32 *hint, const NNSG3dAnmObj *anmObj) {
    while (anmObj != NULL) {
        int i;

        for (i = 0; i < anmObj->numMapData; i++) {
            if (anmObj->mapData[i] & NNS_G3D_ANMOBJ_MAPDATA_EXIST) {
                hint[i >> 5] |= 1 << (i & 0x1f);
            }
        }
        anmObj = anmObj->next;
    }
}

// The object NNS_G3DDrawCore draws with: one the caller of NNS_G3DDraw had going already, or the buffer NNS_G3DDraw
// puts here while it draws, so that a draw started inside that draw reuses it
NNSG3dRenderObj *g_NNSG3DModelRenderer;

void NNS_G3DDraw(NNSG3dRenderObj *obj) {
    u8 work[0x188];

    if ((obj->flag & NNS_G3D_RENDEROBJ_FLAG_HINT_OBSOLETE) == NNS_G3D_RENDEROBJ_FLAG_HINT_OBSOLETE) {
        sys_memset32(0, obj->hintMatAnmExist, sizeof(obj->hintMatAnmExist));
        sys_memset32(0, obj->hintJntAnmExist, sizeof(obj->hintJntAnmExist));
        sys_memset32(0, obj->hintVisAnmExist, sizeof(obj->hintVisAnmExist));
        if (obj->anmMat != NULL) {
            func_020662bc(obj->hintMatAnmExist, obj->anmMat);
        }
        if (obj->anmJnt != NULL) {
            func_020662bc(obj->hintJntAnmExist, obj->anmJnt);
        }
        if (obj->anmVis != NULL) {
            func_020662bc(obj->hintVisAnmExist, obj->anmVis);
        }
        obj->flag &= ~NNS_G3D_RENDEROBJ_FLAG_HINT_OBSOLETE;
    }

    if (g_NNSG3DModelRenderer != NULL) {
        NNS_G3DDrawCore(g_NNSG3DModelRenderer, obj);
    } else {
        g_NNSG3DModelRenderer = (NNSG3dRenderObj *)work;
        NNS_G3DDrawCore(g_NNSG3DModelRenderer, obj);
        g_NNSG3DModelRenderer = NULL;
    }
}
