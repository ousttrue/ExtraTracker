#include <openvr_driver.h>


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
class CServerDriver_Sample : public vr::IServerTrackedDeviceProvider
{
public:
    virtual vr::EVRInitError Init(vr::IVRDriverContext *pDriverContext);
    virtual void Cleanup();
    virtual const char *const *GetInterfaceVersions() { return vr::k_InterfaceVersions; }
    virtual void RunFrame();
    virtual bool ShouldBlockStandbyMode() { return false; }
    virtual void EnterStandby() {}
    virtual void LeaveStandby() {}

private:
    class CSampleDeviceDriver *m_pNullHmdLatest = nullptr;
    class CSampleControllerDriver *m_pController = nullptr;
};
