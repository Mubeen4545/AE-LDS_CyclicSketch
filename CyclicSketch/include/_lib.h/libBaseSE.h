#ifndef LIB_BASE_SE_H
#define LIB_BASE_SE_H

enum packet_statistics_object_type {
    pso_FlowDuration, pso_ActiveMean, pso_IdleMean,
    pso_TotalPackets, pso_TotalBytes, pso_FlowRate,
    pso_AvgPacketSize, pso_StdDevPacketSize,
    pso_PacketInterArrivalMean, pso_PacketInterArrivalStdDev,
    pso_HeaderLength, pso_SelfDefine
};

enum FeatureType {
    fea_FlowDuration=0, fea_ActiveMean, fea_IdleMean,
    fea_TotalPackets, fea_TotalBytes, fea_FlowRate,
    fea_AvgPacketSize, fea_StdDevPacketSize,
    fea_PacketInterArrivalMean, fea_PacketInterArrivalStdDev,
    fea_HeaderLength,
    
    fea_TCPPSHSYN, fea_TCPSYN, fea_TCPSACK, fea_TCPTimestamp,
    
    fea_IPPortHash, fea_IPHash, fea_PortHash, fea_IPPortPairHash,
    fea_IPPortHash8, fea_IPHash8, fea_PortHash8, fea_IPPortPairHash8
};

enum packet_statistics_method {
    psm_Unique=0, psm_TotalFlow, psm_FlowRate, psm_AvgSize, psm_StdDevSize,
    psm_InterArrivalMean, psm_InterArrivalStdDev, psm_filter
};

#endif
