#include "xpcapi.hpp"

#include "xpc_msg.hpp"
#include "xpc_comms.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <filesystem>

using namespace xpcapi;

Connection* con_ = new Connection();
Message* msg_ = new Message();

int err_ = 0;

int xpcapi::xPCReOpenPort(int port) {
    return 0;
}
int xpcapi::xPCOpenSerialPort(int comport, int baudRate) {
    return 0;
}
void xpcapi::xPCClosePort(int port) {
    return;
}
int xpcapi::xPCGetLastError(void) {
    return 0;
}
void xpcapi::xPCSetLastError(int error) {
    return;
}
double xpcapi::xPCGetExecTime(int port) {
    return 0.0;
}
void xpcapi::xPCSetStopTime(int port, double tfinal) {
    return;
}
double xpcapi::xPCGetStopTime(int port) {
    msg_->reset(0x14, 0x0A);
    msg_->send(*con_);

    double time;
    msg_->read(time);
    return time;
}
void xpcapi::xPCSetSampleTime(int port, double ts) {
    return;
}
double xpcapi::xPCGetSampleTime(int port) {
    msg_->reset(0x14, 0x28);
    msg_->send(*con_);

    double time;
    msg_->read(time);
    return time;
}
void xpcapi::xPCSetEcho(int port, int mode) {
    return;
}
int xpcapi::xPCGetEcho(int port) {
    return 0;
}
void xpcapi::xPCSetHiddenScopeEcho(int port, int mode) {
    return;
}
int xpcapi::xPCGetHiddenScopeEcho(int port) {
    return 0;
}
double xpcapi::xPCAverageTET(int port) {
    return 0.0;
}
int xpcapi::xPCGetNumParams(int port) {
    return 0;
}
int xpcapi::xPCGetNumSignals(int port) {
    return 0;
}
char * xpcapi::xPCGetAppName(int port, char *modelname) {
    return nullptr;
}
void xpcapi::xPCUnloadApp(int port) {
    // Uses CheckPort
    msg_->reset(0xC8, 0x3C);
    msg_->send(*con_);

    std::string res;
    msg_->read(res);

    if (res != std::string("loader")) {
        int status = xPCIsAppRunning(port);

        if (status != 0) {
            xPCStopApp(port);
            return;
        }

        msg_->reset(0x14, 0xAC);
        msg_->send(*con_);
    }


}
void xpcapi::xPCStartApp(int port) {
    msg_->reset(0x14, 0x9b);
    uint32_t data = 5;
    msg_->push(&data, 1);
    msg_->send(*con_);
}
void xpcapi::xPCStopApp(int port) {
    msg_->reset(0x14, 0x9b);
    uint32_t data = 1;
    msg_->push(&data, 1);
    msg_->send(*con_);
}
int xpcapi::xPCIsAppRunning(int port) {
    msg_->reset(0x14, 0xA1);
    msg_->send(*con_);

    uint32_t status;
    msg_->read(status);
    return status;
}
int xpcapi::xPCIsOverloaded(int port) {
    return 0;
}
int xpcapi::xPCGetNumOutputs(int port) {
    return 0;
}
int xpcapi::xPCGetNumStates(int port) {
    return 0;
}
void xpcapi::xPCGetParam(int port, int parIdx, double *paramValue) {
    return;
}
void xpcapi::xPCSetLogMode(int port, lgmode lgdata) {
    return;
}
void xpcapi::xPCSetParam(int port, int parIdx, const double *paramValue) {
    return;
}
lgmode xpcapi::xPCGetLogMode(int port) {
    return {};
}
int xpcapi::xPCNumLogSamples(int port) {
    return 0;
}
int xpcapi::xPCMaxLogSamples(int port) {
    return 0;
}
int xpcapi::xPCNumLogWraps(int port) {
    return 0;
}
void xpcapi::xPCReboot(int port) {
    return;
}
void xpcapi::xPCGetOutputLog(int port, int start, int numsamples,
                                  int decimation, int output_id, double *data) {
    return;
}
void xpcapi::xPCGetStateLog(int port, int start, int numsamples,
                                 int decimation, int state_id, double *data) {
    return;
}
void xpcapi::xPCGetTimeLog(int port, int start, int numsamples,
                                int decimation, double *data) {
    return;
}
void xpcapi::xPCGetTETLog(int port, int start, int numsamples,
                               int decimation, double *data) {
    return;
}
void xpcapi::xPCScGetData(int port, int scNum , int signal_id, int start,
                               int numsamples, int decimation, double *data) {
    return;
}
void xpcapi::xPCMinimumTET(int port, double *data) {
    return;
}
void xpcapi::xPCMaximumTET(int port, double *data) {
    return;
}
int xpcapi::xPCGetSignals(int port, int numSignals, const int *signals, double *values) {
    if (numSignals >= 1000) {
        return -1;
    }
    
    msg_->reset(0x28, 0x0A);
    msg_->push<uint32_t>(reinterpret_cast<const uint32_t*>(signals), numSignals);
    msg_->send(*con_);

    msg_->read<double>(values);
}
double xpcapi::xPCGetSignal(int port, int sigNum) {
    double value;
    xPCGetSignals(port, 1, &sigNum, &value);
    return value;
}
void xpcapi::xPCAddScope(int port, int type, int scNum) {
    return;
}
void xpcapi::xPCRemScope(int port, int scNum) {
    return;
}
void xpcapi::xPCScAddSignal(int port, int scNum, int sigNum) {
    return;
}
void xpcapi::xPCScRemSignal(int port, int scNum, int sigNum) {
    return;
}
void xpcapi::xPCScSetAutoRestart(int port, int scNum, int autorestart) {
    return;
}
int xpcapi::xPCScGetAutoRestart(int port, int scNum) {
    return 0;
}
void xpcapi::xPCGetScopes(int port, int *data) {
    return;
}
void xpcapi::xPCGetHiddenScopes(int port, int *data) {
    return;
}
void xpcapi::xPCScGetSignals(int port, int scNum, int *data) {
    return;
}
void xpcapi::xPCScSetDecimation(int port, int scNum, int decimation) {
    return;
}
int xpcapi::xPCScGetNumSignals(int port, int scNum) {
    return 0;
}
int xpcapi::xPCScGetDecimation(int port, int scNum) {
    return 0;
}
void xpcapi::xPCScSetNumSamples(int port, int scNum, int samples) {
    return;
}
int xpcapi::xPCScGetNumSamples(int port, int scNum) {
    return 0;
}
double xpcapi::xPCScGetStartTime(int port, int scNum) {
    return 0.0;
}
int xpcapi::xPCScGetState(int port, int scNum) {
    return 0;
}
void xpcapi::xPCScSetTriggerLevel(int port, int scNum, double level) {
    return;
}
double xpcapi::xPCScGetTriggerLevel(int port, int scNum) {
    return 0.0;
}
void xpcapi::xPCScSetTriggerMode(int port, int scNum, int mode) {
    return;
}
int xpcapi::xPCScGetTriggerMode(int port, int scNum) {
    return 0;
}
void xpcapi::xPCScSetTriggerScope(int port, int scNum, int trigMode) {
    return;
}
int xpcapi::xPCScGetTriggerScope(int port, int scNum) {
    return 0;
}
void xpcapi::xPCScSetTriggerScopeSample(int port, int scNum, int trigScSamp) {
    return;
}
int xpcapi::xPCScGetTriggerScopeSample(int port, int scNum) {
    return 0;
}
void xpcapi::xPCScSetTriggerSignal(int port, int scNum, int trigSig) {
    return;
}
int xpcapi::xPCScGetTriggerSignal(int port, int scNum) {
    return 0;
}
void xpcapi::xPCScSetTriggerSlope(int port, int scNum, int trigSlope) {
    return;
}
int xpcapi::xPCScGetTriggerSlope(int port, int scNum) {
    return 0;
}
void xpcapi::xPCScSoftwareTrigger(int port, int scNum) {
    return;
}
void xpcapi::xPCScStart(int port, int scNum) {
    return;
}
void xpcapi::xPCScStop(int port, int scNum) {
    return;
}
int xpcapi::xPCIsScFinished(int port, int scNum) {
    return 0;
}
int xpcapi::xPCScGetNumPrePostSamples(int port, int scNum) {
    return 0;
}
void xpcapi::xPCScSetNumPrePostSamples(int port, int scNum, int prepost) {
    return;
}
scopedata xpcapi::xPCGetScope(int port, int scNum) {
    return {};
}
void xpcapi::xPCSetScope(int port, scopedata state) {
    return;
}
void xpcapi::xPCLoadApp(int port, const char* pathstr, const char* filename) {
    std::filesystem::path base = pathstr;
    std::filesystem::path path = base / filename;
    if (!path.has_extension()) {
        path = path / ".dlm";
    }
    std::ifstream file(path, std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        err_ = 0x12;
        return;
    }
    std::streamsize size = file.tellg();
    std::vector<char> buf(size);

    file.seekg(0, std::ios::beg);
    file.read(buf.data(), size);

    msg_->reset(0xc8, 0x8c);
    msg_->push(buf.data(), buf.size());
    msg_->send(*con_);

}
void xpcapi::xPCGetParamDims(int port, int parIdx, int *dims) {
    return;
}
int  xpcapi::xPCGetParamDimsSize(int port, int parIdx) {
    return 0;
}
int xpcapi::xPCGetSignalWidth(int port, int sigIdx) {
    return 0;
}
int xpcapi::xPCGetSignalIdx(int port, const char *sigName ) {
    msg_->reset(0x28, 0x46);
    msg_->push<char>(sigName, strlen(sigName) + 1);
    msg_->send(*con_);
    int32_t val;
    msg_->read<int32_t>(val);
    return val;
}

int xpcapi::xPCGetSigLabelWidth(int port, const char *sigName) {
    return 0;
}
int xpcapi::xPCGetSigIdxfromLabel(int port, const char *sigName, int *sigIds) {
    return 0;
}
char * xpcapi::xPCGetSignalLabel(int port, int sigIdx, char *sigLabel) {
    return nullptr;
}

int xpcapi::xPCGetParamIdx(int port, const char *block,
                                const char *parameter) {
    return 0;
}
void xpcapi::xPCGetParamName(int port, int parIdx, char *block, char *param) {
    return;
}
void xpcapi::xPCGetParamType(int port, int parIdx, char *paramType) {
    return;
}
char * xpcapi::xPCGetSignalName(int port, int sigIdx, char *sigName) {
    return nullptr;
}
int xpcapi::xPCTgScGetGrid(int port, int scNum) {
    return 0;
}
int xpcapi::xPCTgScGetMode(int port, int scNum) {
    return 0;
}
int xpcapi::xPCTgScGetViewMode(int port) {
    return 0;
}
void xpcapi::xPCTgScGetYLimits(int port, int scNum, double *limits) {
    return;
}
void xpcapi::xPCTgScSetGrid(int port, int scNum, int flag) {
    return;
}
void xpcapi::xPCTgScSetMode(int port, int scNum, int flag) {
    return;
}
void xpcapi::xPCTgScSetViewMode(int port, int scNum) {
    return;
}
void xpcapi::xPCTgScSetYLimits(int port, int scNum, const double *limits) {
    return;
}
void xpcapi::xPCTgScSetSignalFormat(int port, int scNum, int signalNo, const char *signalFormat) {
    return;
}
char* xpcapi::xPCTgScGetSignalFormat(int port,int scNum, int signalNo, char *signalFormat) {
    return nullptr;
}
void xpcapi::xPCSetLoadTimeOut(int port, int timeOut) {
    return;
}
const char * xpcapi::xPCErrorMsg(int errorno, char *errmsg) {
    return nullptr;
}
int xpcapi::xPCScGetType(int port, int scNum) {
    return 0;
}
int xpcapi::xPCGetLoadTimeOut(int port) {
    return 0;
}
int xpcapi::xPCOpenTcpIpPort(const char* address, const char* port) {
    con_->con_connect(address, std::stoi(port));

    return 0;
}
void xpcapi::xPCOpenConnection(int port) {
    return;
}
void xpcapi::xPCCloseConnection(int port) {
    return;
}
int xpcapi::xPCRegisterTarget(int commType, const char *ipAddress,
                                   const char *ipPort,
                                   int comPort, int baudRate) {
    return 0;
}
void xpcapi::xPCDeRegisterTarget(int port) {
    return;
}
const char * xpcapi::xPCGetAPIVersion(void) {
    return nullptr;
}
void xpcapi::xPCGetTargetVersion(int port, char *ver) {
    return;
}
int xpcapi::xPCTargetPing(int port) {
    return 0;
}
void xpcapi::xPCFSReadFile(int port, int fileHandle, int start,
                               int numsamples, unsigned char *data) {
    return;
}
int xpcapi::xPCFSRead(int port, int fileHandle, int start,
                           int numsamples, unsigned char *data) {
    return 0;
}
void xpcapi::xPCFSWriteFile(int port, int fileHandle, int numbytes,
                                 const unsigned char *data) {
    return;
}
void xpcapi::xPCFSBufferInfo(int port, char *data) {
    return;
}
int xpcapi::xPCFSGetFileSize(int port, int fileHandle) {
    return 0;
}
int xpcapi::xPCFSOpenFile(int port, const char *filename,
                               const char *attrib) {
    return 0;
}
void xpcapi::xPCFSCloseFile(int port, int fileHandle) {
    return;
}
void xpcapi::xPCFSGetPWD(int port, char *data) {
    return;
}
void xpcapi::xPCFTPGet(int port, int fileHandle, int numbytes, char *filename) {
    return;
}
void xpcapi::xPCFTPPut(int port, int fileHandle, char *filename) {
    return;
}
void xpcapi::xPCFSRemoveFile(int port, char *filename) {
    return;
}
void xpcapi::xPCFSCD(int port, char *filename) {
    return;
}
void xpcapi::xPCFSMKDIR(int port, const char *dirname) {
    return;
}
void xpcapi::xPCFSRMDIR(int port, const char *dirname) {
    return;
}
void xpcapi::xPCFSDir(int port, const char *path, char *listing, int numbytes) {
    return;
}
int xpcapi::xPCFSDirSize(int port, const char *path) {
    return 0;
}
void xpcapi::xPCFSGetError(int            port,
                                unsigned int   errCode,
                                unsigned char *message) {
    return;
}
void xpcapi::xPCSaveParamSet(int port, const char *filename) {
    return;
}
void xpcapi::xPCLoadParamSet(int port, const char *filename) {
    return;
}

void xpcapi::xPCFSScSetFilename(int port, int scopeId,
                                     const char *filename) {
    return;
}
const char * xpcapi::xPCFSScGetFilename(int port, int scopeId,
                                             char *filename) {
    return nullptr;
}
void xpcapi::xPCFSScSetWriteMode(int port, int scopeId,
                                      int writeMode) {
    return;
}
int xpcapi::xPCFSScGetWriteMode(int port, int scopeId) {
    return 0;
}

void xpcapi::xPCFSScSetWriteSize(int port, int scopeId,
                                     unsigned int writeSize) {
    return;
}
unsigned int xpcapi::xPCFSScGetWriteSize(int port, int scopeId) {
    return 0;
}
void xpcapi::xPCReadXML(int port, int numbytes, unsigned char *data) {
    return;
}
diskinfo xpcapi::xPCFSDiskInfo(int port, const char *driveLetter) {
    return {};
}
const char * xpcapi::xPCFSFileTable(int port, char *tableBuffer) {
    return nullptr;
}
void xpcapi::xPCFSDirItems(int port, const char *path, dirStruct *dirs, int numDirItems) {
    return;
}
int xpcapi::xPCFSDirStructSize(int port, const char *path) {
    return 0;
}
int xpcapi::xPCGetNumScopes(int port) {
    return 0;
}
int xpcapi::xPCGetNumHiddenScopes(int port) {
    return 0;
}
void xpcapi::xPCGetScopeList(int port, int *data) {
    return;
}
void xpcapi::xPCGetHiddenList(int port, int *data) {
    return;
}
void xpcapi::xPCScGetSignalList(int port, int scNum, int *data) {
    return;
}
int xpcapi::xPCGetSimMode(int port) {
    return 0;
}
void xpcapi::xPCGetPCIInfo(int port, char *buf) {
    return;
}
double xpcapi::xPCGetSessionTime(int port) {
    return 0.0;
}
void xpcapi::xPCGetLogStatus(int port, int *logArray) {
    return;
}
fileinfo xpcapi::xPCFSFileInfo(int port, int fileHandle) {
    return {};
}
void xpcapi::xPCSetDefaultStopTime(int port) {
    return;
}
int xpcapi::xPCGetXMLSize(int port) {
    return 0;
}
int xpcapi::xPCIsTargetScope(int port) {
    return 0;
}
void  xpcapi::xPCSetTargetScopeUpdate(int port,int value) {
    return;
}
void xpcapi::xPCFSReNameFile(int port, const char *fsName, const char *newName) {
    return;
}
void xpcapi::xPCFSScSetDynamicMode(int port, int scopeId, int onoff) {
    return;
}
int xpcapi::xPCFSScGetDynamicMode(int port, int scopeId) {
    return 0;
}
void xpcapi::xPCFSScSetMaxWriteFileSize(int port, int scopeId,
                                     unsigned int maxWriteFileSize) {
    return;
}
unsigned int xpcapi::xPCFSScGetMaxWriteFileSize(int port, int scopeId) {
    return 0;
}