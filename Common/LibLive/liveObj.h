#pragma once

#include "libLive.h"
#include "uv.h"

enum NalType;

/**
 * RTSP����ģ��ӿ�
 * ʹ��ǰ����������ȳ�ʼ��<ָUdpSocket�����ʼ������ʹ��>
 */
class CLiveObj : public IlibLive
{
public:
    CLiveObj(void);
    ~CLiveObj(void);

    /**
     * ���ñ��ؼ�����IP�Ͷ˿�
     * @param[in] strIP ����IP
     * @param[in] nPort ������UDP�˿�
     */
    void SetLocalAddr(string strIP, int nPort)
    {
        m_strLocalIP = strIP;
        m_nLocalRTPPort = nPort;
        m_nLocalRTCPPort = nPort + 1;
    }

    /**
     * ���û���֡����
     * @param[in] nPacketNum ֡��������,��ֵԽ���ӳ�Խ�󣬵���Ӧ�Ը��������״��
     */
    void SetCatchPacketNum(int nPacketNum);

    /** ����UDP�˿ڼ��� */
    void StartListen();

    /** ���յ�rtp���ݴ��� */
    void RtpRecv(char* pBuff, long nLen);

    /** ���ճ�ʱ���� */
    void RtpOverTime();

    /**
     * RTP����ص�
     * @param[in] pBuff PS֡����
     * @param[in] nLen PS֡����
     */
    void RTPParseCb(char* pBuff, long nLen);

    /**
     * PS֡�����ص�
     * @param[in] pBuff PES������
     * @param[in] nLen PES������
     */
    void PSParseCb(char* pBuff, long nLen);

    /**
     * PES֡�����ص�
     * @param[in] pBuff ES������
     * @param[in] nLen ES������
     * @param[in] pts չ��ʱ����ֶ�
     * @param[in] dts ����ʱ����ֶ�
     */
    void PESParseCb(char* pBuff, long nLen, uint64_t pts, uint64_t dts);

    /**
     * ES֡�����ص�
     * @param[in] pBuff H264֡����
     * @param[in] nLen H264֡����
     * @param[in] nNalType Nalu������
     */
    void ESParseCb(char* pBuff, long nLen/*, uint8_t nNalType*/);

    /** H264��sps�����ص� */
    void H264SpsCb(uint32_t nWidth, uint32_t nHeight, double fFps);

    /** FLV�ϳɻص� */
    void FlvCb(FLV_FRAG_TYPE eType, char* pBuff, int nBuffSize);

    /** MP4�ϳɻص� */
    void Mp4Cb(MP4_FRAG_TYPE eType, char* pBuff, int nBuffSize);

    /** TS�ϳɻص� */
    void TsCb(char* pBuff, int nBuffSize);

    /** H264�ϳɻص� */
    void H264Cb(char* pBuff, int nBuffSize);

    /**
     * ���ô������ݻص��Ķ���
     * @param[in] pHandle
     */
    void SetCallback(IlibLiveCb* pHandle)
    {
        m_pCallBack = pHandle;
    }

private:
    string      m_strLocalIP;       // ����IP
    int         m_nLocalRTPPort;    // ����RTP�˿�
    int         m_nLocalRTCPPort;   // ����RTCP�˿�
    string      m_strRemoteIP;      // Զ��IP
    int         m_nRemoteRTPPort;   // Զ��RTP�˿�
    int         m_nRemoteRTCPPort;  // Զ��RTCP�˿�

    uv_udp_t    m_uvRtpSocket;      // rtp����
    uv_timer_t  m_uvTimeOver;       // ���ճ�ʱ��ʱ��

    void*       m_pRtpParser;       // rtp���Ľ�����
    void*       m_pPsParser;        // PS֡������
    void*       m_pPesParser;       // PES��������
    void*       m_pEsParser;        // ES��������
    void*       m_pH264;            // H264������
    void*       m_pTs;              // TS�����
    void*       m_pFlv;             // FLV�����
    void*       m_pMp4;             // MP4�����
    IlibLiveCb* m_pCallBack;        // �ص�����

    uint64_t    m_pts;              // ��¼PES�е�pts
    uint64_t    m_dts;              // ��¼PES�е�dts
    NalType     m_nalu_type;        // h264ƬԪ����
};
