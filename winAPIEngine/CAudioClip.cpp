#include "CAudioClip.h"

HRESULT CAudioClip::Load(CAPIEngine* tEngine, const std::wstring& tPath)
{
    std::string cPath(tPath.begin(), tPath.end());
    if (!CFMOD::CreateSound(cPath, mSound))
        return S_FALSE;

    return E_NOTIMPL;
}
