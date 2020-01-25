#include <openvr_driver.h>

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
class CSampleControllerDriver : public vr::ITrackedDeviceServerDriver
{
public:
    CSampleControllerDriver();
    virtual ~CSampleControllerDriver() {}
    virtual vr::EVRInitError Activate(vr::TrackedDeviceIndex_t unObjectId);
    virtual void Deactivate()
    {
        m_unObjectId = vr::k_unTrackedDeviceIndexInvalid;
    }
    virtual void EnterStandby() {}
    void *GetComponent(const char *pchComponentNameAndVersion)
    {
        // override this to add a component to a driver
        return NULL;
    }
    virtual void PowerOff() {}
    /** debug request from a client */
    virtual void DebugRequest(const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize);
    virtual vr::DriverPose_t GetPose();
    void RunFrame();
    void ProcessEvent(const vr::VREvent_t &vrEvent);

    std::string GetSerialNumber() const { return m_sSerialNumber; }

private:
    vr::TrackedDeviceIndex_t m_unObjectId;
    vr::PropertyContainerHandle_t m_ulPropertyContainer;

    vr::VRInputComponentHandle_t m_compA;
    vr::VRInputComponentHandle_t m_compB;
    vr::VRInputComponentHandle_t m_compC;
    vr::VRInputComponentHandle_t m_compHaptic;

    std::string m_sSerialNumber;
    std::string m_sModelNumber;
};
