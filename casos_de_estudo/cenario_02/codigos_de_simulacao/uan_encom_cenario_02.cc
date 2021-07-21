/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as 
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Leonard Tracy <lentracy@gmail.com>
 */

/**
 * \file uan-rc-example.cc
 * \ingroup UAN
 * This example uses UanMacRc and UanMacRcGw which combined form a system
 * using what is referred to as RC-MAC.  Details of RC-MAC will be published
 * soon.  In brief terms, RC-MAC is a dual channel protocol wherein the
 * available bandwidth is dynamically divided into two channels,
 * a reservation channel and a data channel.  The network is assumed
 * to consist of a single gateway node which services several
 * non-gateway nodes.
 * 
 * Time is divided into cycles.  The non-gateway nodes transmit RTS packets
 * on the reservation channel in parallel to scheduled data transmissions
 * (scheduled in the previous cycle), and the gateway stores these requests
 * for the duration of the cycle.  At the start of the next cycle
 * the gateway node transmits a CTS which contains packet transmission times
 * for reserved packets as well as bandwidth allocation information
 * 
 * This script deploys a single gateway node (current UanMacRc only supports
 * a single gateway) in the center of a region and then distributes
 * non-gateway nodes around the gateway with a uniformly distributed range
 * between each node and the gateway.
 * 
 * The script supports two simulation types.  By default the gateway
 * dynamically determines the optimal parameter settings and
 * simulations are run with varying number of nodes (SimMin to SimMax as
 * set by the command line).  If DoNode=0 is given as a command line option
 * then the mac parameter "a" (approximate expected number of successful
 * RTS arrivals per cycle) is varied as the simulation parameter.
 * 
 * For questions about this MAC protocol email "lentracy@gmail.com"
 */

#include "uan-auv-ipv4-npad.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/log.h"
#include "ns3/config.h"
#include "ns3/callback.h"
#include "ns3/stats-module.h"
#include "ns3/random-variable-stream.h"

// Modules for AUV
#include "ns3/basic-energy-source-helper.h"
#include "ns3/auv-waypoint-mobility-model.h"
#include "ns3/basic-energy-source.h"

//TODO Inserir bibliotecas para os outros modelos de auv

// Modules for AUV Glider model
#include "ns3/auv-glider-helper.h"
#include "ns3/glider-energy-model.h"
#include "ns3/glider-constraints.h"

// Modules for AUV Remus model
#include "ns3/auv-remus-helper.h"
#include "ns3/remus-energy-model.h"
#include "ns3/remus-constraints.h"

// Modules for AUV Typhoon model
#include "ns3/auv-typhoon-helper.h"
#include "ns3/typhoon-energy-model.h"
#include "ns3/typhoon-constraints.h"

// INTERNET MODULES
#include "ns3/internet-module.h"
#include "ns3/ipv4-flow-classifier.h"
#include "ns3/ipv4-l3-protocol.h"

// FLOW MONITOR MODULE
#include "ns3/flow-monitor-module.h"
#include "ns3/flow-monitor-helper.h" //flow monitor
#include "ns3/flow-probe.h"

// UAN Modules

#include "ns3/uan-noise-model-default.h"
#include "ns3/uan-prop-model-ideal.h"
#include "ns3/uan-prop-model-thorp.h"

// configStore
#include "ns3/config-store-module.h"


#include <fstream>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("UanAuvAlohaExample");

//TODO: Adicionar variável para escolher o modelo do AUV (Foi preciso ir no .h para
//adicionar a variavel no escopo da classe Experiment)

Experiment::Experiment ()
  : m_nsinks (1),
    m_numNodes (0), //nodes
	  m_auvs(1),
	  m_auvModel(3),
    m_maxRange (200),
    m_maxDepth (100),
    m_pktSize (32),
    m_simTime (Days (1)),
	  m_filename("CampaignUan"),
    m_propModel("UanPropModelIdeal"),
	  m_noiseModel("UanNoiseModelDefault"),
    m_outputdir("./"),
    m_bytesTotal (0)
{ 
}


void
Experiment::PrintReceivedPacket (Ptr<Socket> socket)
{
  Address srcAddress;
  while (socket->GetRxAvailable () > 0)
    {
      Ptr<Packet> packet = socket->RecvFrom (srcAddress);
      uint8_t energyReading;
      packet->CopyData (&energyReading, 1);

      if(InetSocketAddress::IsMatchingType (srcAddress))
        {
          //NS_LOG_UNCOND ( "Time: " << Simulator::Now ().GetHours () << "h" << " | Node: " <<
          //                InetSocketAddress::ConvertFrom (srcAddress).GetIpv4 () << " | Energy: " <<
          //                +energyReading << "%");
          NS_LOG_UNCOND ( "Time: " << Simulator::Now ().GetHours () << "h" << " | Node: " <<
                          InetSocketAddress::ConvertFrom (srcAddress).GetIpv4 ());
        }
    }
}

void
Experiment::PrintReceivedPacketAuv (Ptr<Socket> socket)
{
  Address srcAddress;
  while (socket->GetRxAvailable () > 0)
    {
      Ptr<Packet> packet = socket->RecvFrom (srcAddress);
      uint8_t energyReading;
      packet->CopyData (&energyReading, 1);

      if(InetSocketAddress::IsMatchingType (srcAddress))
        {
          NS_LOG_UNCOND ( "Time: " << Simulator::Now ().GetHours () << "h" << " | AUV: " <<
                          InetSocketAddress::ConvertFrom (srcAddress).GetIpv4 () << " | Energy: " <<
                          +energyReading << "%");
        }
    }
}


void
Experiment::SendSinglePacket (Ptr<Node> node, Ptr<Packet> pkt, Ipv4Address dst, Ipv4Address orgn)
{
  //NS_LOG_UNCOND ( Simulator::Now ().GetHours () << "h" << " packet sent from " <<  orgn << " to " << dst);
  InetSocketAddress ipv4_destination = InetSocketAddress (dst, 9);
  m_sockets[node]->SendTo (pkt, 0, ipv4_destination);
  //m_sockets[node]->SetRecvCallback (MakeCallback (&Experiment::PrintReceivedPacket, this));
}

void
Experiment::SendSinglePacketAuv (Ptr<Node> node, Ptr<Packet> pkt, Ipv4Address dst, Ipv4Address orgn)
{
  //NS_LOG_UNCOND ( Simulator::Now ().GetHours () << "h" << " packet sent from " <<  orgn << " to " << dst);
  InetSocketAddress ipv4_destination = InetSocketAddress (dst, 9);
  m_sockets_auv[node]->SendTo (pkt, 0, ipv4_destination);
  //m_sockets[node]->SetRecvCallback (MakeCallback (&Experiment::PrintReceivedPacket, this));
}

uint32_t
Experiment::Run (uint32_t nNodes)
{

  static bool verbose = true;
  m_bytesTotal=0;
  Time pDelay = Seconds ((double) m_maxRange / 1500.0);

  /**********************SETUP NODES*********************************/
  NodeContainer sinks,nodes,auv;
  sinks.Create (1);
  nodes.Create (nNodes);
  auv.Create(m_auvs);
  /**********************SETUP POSITIONS*****************************/

  MobilityHelper mobility;
  uint32_t depth = 0;
  uint32_t length = 0;
  uint32_t width = 0;
  Ptr<ListPositionAllocator> pos = CreateObject<ListPositionAllocator> ();
  Ptr<UniformRandomVariable> urv = CreateObject<UniformRandomVariable> ();
  Ptr<UniformRandomVariable> utheta = CreateObject<UniformRandomVariable> ();
  pos->Add (Vector (length, width, depth));

  mobility.SetPositionAllocator (pos);
  mobility.Install (sinks);
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
		  	  	  	  	  	  	  "MinX", DoubleValue (0.0),
		                          "MinY", DoubleValue (0.0),
		                          "DeltaX", DoubleValue (5.0),
		                          "DeltaY", DoubleValue (5.0),
								              "GridWidth", UintegerValue (3),
		  	  	  	  	  	  	  "LayoutType", StringValue ("RowFirst"));

  mobility.Install (nodes);


  uint32_t auv_depth = m_maxDepth;
  uint32_t auv_length = m_maxRange;

  if (m_auvModel == 1){
	  AuvGliderHelper re;
	  re.Install(auv);
  }
  else if (m_auvModel == 2){
	  AuvRemusHelper re;
	  re.Install(auv); 

  }
  else if (m_auvModel == 3){
	  AuvTyphoonHelper re;
	  re.Install(auv);
  }

  Ptr<AuvWaypointMobilityModel> mob = auv.Get(0)->GetObject<AuvWaypointMobilityModel> ();
  mob->AddWaypoint (Waypoint (Seconds (0.0), Vector (0,0,auv_depth)));
  mob->AddWaypoint (Waypoint (Seconds (43500.0), Vector (auv_length,0,auv_depth)));
  mob->AddWaypoint (Waypoint (Seconds (86400.0), Vector (0,0,auv_depth)));

  Ptr<DeviceEnergyModel> energyModel;
  if(m_auvModel == 1){
	  energyModel = auv.Get(0)->GetObject<EnergySourceContainer> ()->Get (0)->FindDeviceEnergyModels ("ns3::GliderEnergyModel").Get (0);
  }
  else if(m_auvModel == 2){
  	energyModel = auv.Get(0)->GetObject<EnergySourceContainer> ()->Get (0)->FindDeviceEnergyModels ("ns3::RemusEnergyModel").Get (0);
  }
  else if(m_auvModel == 3){
    energyModel = auv.Get(0)->GetObject<EnergySourceContainer> ()->Get (0)->FindDeviceEnergyModels ("ns3::TyphoonEnergyModel").Get (0);
  }

  Ptr<EnergySource> energysrc = auv.Get(0)->GetObject<EnergySourceContainer> ()->Get (0);
  Ptr<BasicEnergySource> basicSourcePtr = DynamicCast<BasicEnergySource> (energysrc);

  /**********************SETUP COMMUNICATIONS************************/
  
  Ptr<UanChannel> chan = CreateObject<UanChannel> ();

  if(m_propModel == "UanPropModelThorp"){
	  chan->SetPropagationModel (CreateObject<UanPropModelThorp> ());
  }else{
	  chan->SetPropagationModel (CreateObject<UanPropModelIdeal> ());
  };

  if(m_noiseModel == "UanNoiseModelDefault"){
	  Ptr<UanNoiseModelDefault> noise = CreateObject<UanNoiseModelDefault> ();
	  chan->SetNoiseModel (noise);
  };

  UanHelper uan;
  NetDeviceContainer sinkDev = uan.Install (sinks, chan);
  NetDeviceContainer devices = uan.Install (nodes, chan);
  NetDeviceContainer auvDev = uan.Install (auv, chan);

  EnergySourceContainer energySourceContainer;
  NodeContainer::Iterator iauv_energy = auv.Begin ();
  while (iauv_energy != auv.End ())
    {
      energySourceContainer.Add ((*iauv_energy)->GetObject<EnergySourceContainer> ()->Get (0));
      iauv_energy++;
    }
  
  AcousticModemEnergyModelHelper acousticModemEnergyModelHelper;
  DeviceEnergyModelContainer deviceModels = acousticModemEnergyModelHelper.Install (auvDev, energySourceContainer);
 
  InternetStackHelper internetStackHelper;
  internetStackHelper.Install (nodes); //Nodes
  internetStackHelper.Install (sinks); // SINK
  internetStackHelper.Install (auv); // SINK

  Ipv4AddressHelper ipv4AddressHelper;
  ipv4AddressHelper.SetBase ("10.0.0.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = ipv4AddressHelper.Assign (devices); //Nodes
  Ipv4InterfaceContainer interfaces_auv = ipv4AddressHelper.Assign (auvDev); //sinks
  Ipv4InterfaceContainer interfaces_sink = ipv4AddressHelper.Assign (sinkDev); //sinks

  NodeContainer::Iterator inode_comm = nodes.Begin ();
  while (inode_comm != nodes.End ())
    {
      (*inode_comm)->GetObject<Ipv4L3Protocol> ()->GetInterface (1)->GetArpCache ()->SetWaitReplyTimeout (Seconds (10));
      inode_comm++;
    }
  NodeContainer::Iterator isink_comm = sinks.Begin();
  while (isink_comm != sinks.End ())
      {
        (*isink_comm)->GetObject<Ipv4L3Protocol> ()->GetInterface (1)->GetArpCache ()->SetWaitReplyTimeout (Seconds (10));
        isink_comm++;
      }

  NodeContainer::Iterator iauv_comm = auv.Begin();
    while (iauv_comm != auv.End ())
        {
          (*iauv_comm)->GetObject<Ipv4L3Protocol> ()->GetInterface (1)->GetArpCache ()->SetWaitReplyTimeout (Seconds (10));
          iauv_comm++;
        }

  /**********************SETUP APPLICATIONS**************************/
  NodeContainer::Iterator inode_app = nodes.Begin ();
  while (inode_app != nodes.End ())
    {
      m_sockets[*inode_app] = Socket::CreateSocket (*inode_app, TypeId::LookupByName ("ns3::UdpSocketFactory"));
      if((*inode_app)->GetObject<Ipv4> () != NULL)
        {
          InetSocketAddress ipv4_local = InetSocketAddress (Ipv4Address::GetAny (), 9);
          m_sockets[*inode_app]->Bind (ipv4_local);
        }

      //m_sockets[*inode_app]->SetRecvCallback (MakeCallback (&Experiment::PrintReceivedPacket, this));
      inode_app++;
    }

  NodeContainer::Iterator isink_app = sinks.Begin();

  while (isink_app != sinks.End ())
    {
      m_sockets_sink[*isink_app] = Socket::CreateSocket (*isink_app, TypeId::LookupByName ("ns3::UdpSocketFactory"));
      if((*isink_app)->GetObject<Ipv4> () != NULL)
        {
          InetSocketAddress ipv4_local = InetSocketAddress (Ipv4Address::GetAny (), 9);
          m_sockets_sink[*isink_app]->Bind (ipv4_local);
        }

      //m_sockets_sink[*isink_app]->SetRecvCallback (MakeCallback (&Experiment::PrintReceivedPacket, this));
      isink_app++;
    }

  NodeContainer::Iterator iauv_app = auv.Begin();

  while (iauv_app != auv.End ())
    {
      m_sockets_auv[*iauv_app] = Socket::CreateSocket (*iauv_app, TypeId::LookupByName ("ns3::UdpSocketFactory"));
      if((*iauv_app)->GetObject<Ipv4> () != NULL)
        {
          InetSocketAddress ipv4_local = InetSocketAddress (Ipv4Address::GetAny (), 9);
          m_sockets_auv[*iauv_app]->Bind (ipv4_local);
        }

      //m_sockets_auv[*iauv_app]->SetRecvCallback (MakeCallback (&Experiment::PrintReceivedPacketAuv, this));
      iauv_app++;
    }


  /**********************SEND PACKETS********************************/
   double mean = 3.14;
   double bound = 0.0;
   Ptr<ExponentialRandomVariable> x = CreateObject<ExponentialRandomVariable> ();
   x->SetAttribute ("Mean", DoubleValue (mean));
   x->SetAttribute ("Bound", DoubleValue (bound));

   Ptr<Packet> pkt = Create<Packet> (m_pktSize);
   Ptr<UniformRandomVariable> uniformRandomVariable = CreateObject<UniformRandomVariable> ();
   double time = uniformRandomVariable->GetValue (0, 60);
   double inter_arrival_time= 0;
   // SEND PACKETS FROM FIXED NODES TO GATEWAY

   NodeContainer::Iterator inode_pkt = nodes.Begin ();

   while (inode_pkt != nodes.End()){
	   NodeContainer::Iterator isink_pkt = sinks.Begin ();
	   while(isink_pkt != sinks.End()){
		 Ipv4Address dst = (*isink_pkt)->GetObject<Ipv4L3Protocol> ()->GetInterface (1)->GetAddress (0).GetLocal ();
         Ipv4Address orgn = (*inode_pkt)->GetObject<Ipv4L3Protocol> ()->GetInterface (1)->GetAddress (0).GetLocal ();
         while(inter_arrival_time <= m_simTime.GetDouble()/1e9){
			   		    	  double expValue=x->GetValue ();
			   		    	  inter_arrival_time = inter_arrival_time + time+expValue;
			   		    	  Simulator::Schedule (Seconds (inter_arrival_time), &Experiment::SendSinglePacket, this, *inode_pkt, pkt, dst, orgn);

			   }
		   if(isink_pkt == sinks.End()){
		   } else{
			   isink_pkt++;
		   }
		   inter_arrival_time=0;
	   }
	   inode_pkt++;
   }
   inter_arrival_time = 0;

   // SEND PACKETS FROM AUVS TO GATEWAY
   NodeContainer::Iterator iauv_pkt = auv.Begin ();
   while (iauv_pkt != auv.End()){
	   NodeContainer::Iterator isink_pkt2= sinks.Begin ();
	   while(isink_pkt2 != sinks.End()){
		 Ipv4Address dst = (*isink_pkt2)->GetObject<Ipv4L3Protocol> ()->GetInterface (1)->GetAddress (0).GetLocal ();
         Ipv4Address orgn = (*iauv_pkt)->GetObject<Ipv4L3Protocol> ()->GetInterface (1)->GetAddress (0).GetLocal ();
         while(inter_arrival_time <= m_simTime.GetDouble()/1e9){
			   		    	  double expValue=x->GetValue ();
			   		    	  inter_arrival_time = inter_arrival_time + time+expValue;
			   		    	  Simulator::Schedule (Seconds (inter_arrival_time), &Experiment::SendSinglePacketAuv, this, *iauv_pkt, pkt, dst, orgn);

			   }
		   if(isink_pkt2 == sinks.End()){
		   } else{
			   isink_pkt2++;
		   }
		   inter_arrival_time=0;
	   }
	   iauv_pkt++;
   }



  /**********************DATA SINK*************************************/
  AsciiTraceHelper asciiTraceHelper;
  
  // Flow monitor
  Ptr<FlowMonitor> flowMonitor;
  FlowMonitorHelper flowHelper;
  flowMonitor = flowHelper.InstallAll();

  // Output config store to txt format
  /*Config::SetDefault ("ns3::ConfigStore::Filename", StringValue ("output-attributes.txt"));
  Config::SetDefault ("ns3::ConfigStore::FileFormat", StringValue ("RawText"));
  Config::SetDefault ("ns3::ConfigStore::Mode", StringValue ("Save"));
  ConfigStore outputConfig2;
  outputConfig2.ConfigureDefaults ();
  outputConfig2.ConfigureAttributes ();*/
  //ConfigStore config;
  //config.ConfigureAttributes ();

  //Simulator::Stop (m_simTime + Seconds (0.6));
  Simulator::Stop (m_simTime);
  Simulator::Run ();

  //TODO: Adicionar informação do modelo do AUV
  std::string sMaxRange = std::to_string (m_maxRange); 
  std::string sMaxDepth = std::to_string (m_maxDepth);
  std::string sAuvModel = std::to_string (m_auvModel);
  std::string dl_results,ul_results;
  dl_results = m_outputdir + "/" + "DL_Results_Sim_PropModel_"+ m_propModel+ "_MaxDepth_"+ sMaxDepth + "_"+"AuvModel_"+sAuvModel +".txt";
  ul_results = m_outputdir + "/" + "UL_Results_Sim_PropModel_"+ m_propModel+ "_MaxDepth_"+ sMaxDepth + "_"+"AuvModel_"+sAuvModel +".txt";


  Ptr<OutputStreamWrapper> DLstreamMetricsInit = asciiTraceHelper.CreateFileStream((dl_results));
  *DLstreamMetricsInit->GetStream()
              << "Flow_ID, Lost_Packets, Tx_Packets, Tx_Bytes, TxOffered(Mbps),  Rx_Packets, Rx_Bytes, T_put(Mbps), Mean_Delay_Rx_Packets, Mean_Jitter, Packet_Loss_Ratio, NavComsumption, ModemComsumption"
              << std::endl;

  Ptr<OutputStreamWrapper> ULstreamMetricsInit = asciiTraceHelper.CreateFileStream((ul_results));
  *ULstreamMetricsInit->GetStream()
              << "Flow_ID, Lost_Packets, Tx_Packets, Tx_Bytes, TxOffered(Mbps),  Rx_Packets, Rx_Bytes, T_put(Mbps), Mean_Delay_Rx_Packets, Mean_Jitter, Packet_Loss_Ratio, NavComsumption, ModemComsumption"
              << std::endl;

  double statDurationTX = 0;
  double statDurationRX = 0;
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowHelper.GetClassifier());

  std::map<FlowId, FlowMonitor::FlowStats> stats = flowMonitor->GetFlowStats();
  DeviceEnergyModelContainer::Iterator iterDevice = deviceModels.Begin ();

  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter =
      stats.begin(); iter != stats.end(); ++iter)
    {
      // some metrics calculation
      statDurationRX = iter->second.timeLastRxPacket.GetSeconds()
                    - iter->second.timeFirstTxPacket.GetSeconds();

      statDurationTX = iter->second.timeLastTxPacket.GetSeconds()
                    - iter->second.timeFirstTxPacket.GetSeconds();

      double meanDelay, meanJitter, packetLossRatio, txTput, rxTput,NavComsumption,ModemComsumption;
      if (iter->second.rxPackets > 0)
        {
          meanDelay = (iter->second.delaySum.GetSeconds()
              / iter->second.rxPackets);
        }
      else // this value is set to zero because the STA is not receiving any packet
        {
          meanDelay = 0;
        }
      //
      if (iter->second.rxPackets > 1)
        {
          meanJitter = (iter->second.jitterSum.GetSeconds()
              / (iter->second.rxPackets - 1));
        }
      else // this value is set to zero because the STA is not receiving any packet
        {
          meanJitter = 0;
        }
      //
      if (statDurationTX > 0)
        {
          txTput = iter->second.txBytes * 8.0 / statDurationTX / 1000 / 1000;
        }
      else
        {
          txTput = 0;
        }
      //
      if (statDurationRX > 0)
        {
          rxTput = iter->second.rxBytes * 8.0 / statDurationRX / 1000 / 1000;
        }
      else
        { energyModel->GetTotalEnergyConsumption ();
          rxTput = 0;
        }
      //
      if ((iter->second.lostPackets > 0) & (iter->second.rxPackets > 0))
        {
          packetLossRatio = (double) (iter->second.lostPackets
              / (double) (iter->second.rxPackets + iter->second.lostPackets));
        }
      else
        {
          packetLossRatio = 0;
        }
       if(iter->first == auv.Get(0)->GetId()){
    	   NavComsumption = energyModel->GetTotalEnergyConsumption ();
         ModemComsumption = (*iterDevice)->GetTotalEnergyConsumption ();
       }else{
    	   NavComsumption=0;
    	   ModemComsumption=0;
       }
      //
      Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(iter->first);
      //
      if (verbose)
             {
               // Print information only if enabled
               //
               //          std::cout << "Flow ID: " << iter->first << ", Source Port: "
               //              << t.sourcePort << ", Destination Port: " << t.destinationPort
               //              << " (" << t.sourceAddress << " -> " << t.destinationAddress
               //              << ")" << std::endl;
               //
               NS_LOG_UNCOND( "\nFlow ID: " << iter->first << ", Source Port: "
                   << t.sourcePort << ", Destination Port: " << t.destinationPort
                   << " (" << t.sourceAddress << " -> " << t.destinationAddress
                   << ")\n" << "\nRESULTS:");
               //
               NS_LOG_UNCOND( "Lost Packets ---------: " << iter->second.lostPackets);
               //
               NS_LOG_UNCOND( "Tx Packets -----------: " << iter->second.txPackets);
               //
               NS_LOG_UNCOND( "Tx Bytes -------------: " << iter->second.txBytes);
               //
               NS_LOG_UNCOND( "TxOffered ------------: " << txTput << " Mbps");
               //std::cout << "TxOffered = " << txTput << " Mbps" << std::endl;
               //
               NS_LOG_UNCOND( "Rx Packets -----------: " << iter->second.rxPackets);
               //
               NS_LOG_UNCOND( "Rx Bytes -------------: " << iter->second.rxBytes);
               //
               NS_LOG_UNCOND( "T-put ----------------: " << rxTput << " Mbps");
               //std::cout << "T-put = " << rxTput << " Mbps" << std::endl;
               //
               NS_LOG_UNCOND( "Mean Delay Rx Packets : " << meanDelay << " s");
               //std::cout << "Mean Delay Rx Packets = " << meanDelay << " s"
               //    << std::endl;
               //
               NS_LOG_UNCOND( "Mean jitter ----------: " << meanJitter << " s");
               //std::cout << "Mean jitter = " << meanJitter << " s" << std::endl;
               //
               NS_LOG_UNCOND( "Packet loss ratio ----: " << packetLossRatio);
               //std::cout << "Packet loss ratio = " << packetLossRatio << std::endl;
               //
               NS_LOG_UNCOND( "\nNav Energy Consumption -----------: "<< NavComsumption << " J");
               //std::cout << "Nav Energy Consumption (done inside auv helper): " <<  energyModel->GetTotalEnergyConsumption ()<<std::endl;
               //
               NS_LOG_UNCOND("Modem Energy Consumption --: "<< ModemComsumption << " J");
               //std::cout << "Nav+Modem Energy Consumption (done inside source helper): " <<  basicSourcePtr ->GetInitialEnergy() - basicSourcePtr -> GetRemainingEnergy() << std::endl;
               //
             }
           //
           Ptr<OutputStreamWrapper> streamMetricsInit = NULL;
           // Get file pointer for DL, if DL flow (using port and IP address to assure correct result)
           //std::cout << "t destination port " << t.destinationPort  <<std::endl;
           //std::cout << "source address " << interfaces.GetAddress(0)  <<std::endl;
           //std::cout << "source address " << t.sourceAddress  <<std::endl;

           //std::cout << "t destination port " << t.destinationPort  <<std::endl;
           //std::cout << "sink address " << interfaces_sink.GetAddress(0)  <<std::endl;
           //std::cout << "destination address " << t.destinationAddress  <<std::endl;
           if ((t.destinationPort == 9)
               & (t.sourceAddress == interfaces_sink.GetAddress(0)))
             {
               streamMetricsInit = DLstreamMetricsInit;
             }
           // Get file pointer for UL, if UL flow (using port and IP address to assure correct result))
           else if ((t.destinationPort == 9)
               & (t.destinationAddress == interfaces_sink.GetAddress(0)))
             {
               streamMetricsInit = ULstreamMetricsInit;
             }
           //
           if (streamMetricsInit)
             {

               /*    	  for (uint32_t reasonCode = 0; reasonCode < iter->second.packetsDropped.size (); reasonCode++)
         	  {
         		  std::cout << "<packetsDropped reasonCode=\"" << reasonCode << "\""
         				  << " number=\"" << iter->second.packetsDropped[reasonCode] << std::endl;

         	  }
         	  for (uint32_t reasonCode = 0; reasonCode < iter->second.bytesDropped.size (); reasonCode++)
         	  {
         		  std::cout << "<bytesDropped reasonCode=\"" << reasonCode << "\""
         				  << " bytes=\"" << iter->second.bytesDropped[reasonCode] << std::endl;
         	  }
                */

               *streamMetricsInit->GetStream() << (iter->first ) << ", "
                   << (iter->second.lostPackets) << ", "
                   //
                   << (iter->second.txPackets) << ", "
                   //
                   << (iter->second.txBytes) << ", "
                   //
                   << txTput << ", "
                   //
                   << (iter->second.rxPackets) << ", "
                   //
                   << (iter->second.rxBytes) << ", "
                   //
                   << rxTput << ", "
                   //
                   << meanDelay << ", "
                   //
                   << meanJitter << ", "
                   //
                   << packetLossRatio << ", "
                   //
                   << NavComsumption << ", "
                   //
                   << ModemComsumption
                   //
                   << std::endl;
             }
           else
             {
               //TODO: chance for an ASSERT
               if ( true )
                 {
                   std::cout << "Some problem to save metrics" << std::endl;
                   std::cout << "Flow ID: " << iter->first << ", Source Port: "
                       << t.sourcePort << ", Destination Port: " << t.destinationPort
                       << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")" << std::endl;
                   std::cout << "AP Address: " << interfaces_sink.GetAddress(0) << std::endl;
                   std::cout << "DLport: " << 9 << std::endl;
                   std::cout << "ULport: " << 9 << std::endl;
                 }
             }

           m_bytesTotal =+ iter->second.rxPackets;
    }


  flowMonitor->SerializeToXmlFile("Sim_PropModel_"+ m_propModel+ "_MaxDepth_" + sMaxDepth +".xml" , true, true);



  Simulator::Destroy ();

  return m_bytesTotal;
}

// To be used in tic toc time counter
	clock_t startTimer;
	time_t beginTimer;
	//
	// Implementation of tic, i.e., start time counter
	void
	tic()
	{
		beginTimer = time(&beginTimer);
		struct tm * timeinfo;
		timeinfo = localtime(&beginTimer);
		std::cout << "simulation start at: " << asctime(timeinfo) << std::endl;
	}
	// implementation of toc, i.e., stop time counter
	double
	toc()
	{
		time_t finishTimer = time(&finishTimer);
		double simTime = difftime(finishTimer, beginTimer) / 60.0;
		struct tm * timeinfo;
		timeinfo = localtime(&finishTimer);
		std::cout << "simulation finished at: " << asctime(timeinfo) << std::endl;
		//
		std::cout << "Time elapsed: " << simTime << " minutes" << std::endl;
		//
		return simTime;
	}


//TODO: Adicionar a variável para modelo do AUV
static ns3::GlobalValue g_numSinks("nSinks",
                              "Number of fixed Sinks",
                              ns3::UintegerValue (1),
                              ns3::MakeUintegerChecker<uint32_t> ());
static ns3::GlobalValue g_numNodes("nNodes",
                              "Number of fixed Nodes",
                              ns3::UintegerValue (0),
                              ns3::MakeUintegerChecker<uint32_t> ());
static ns3::GlobalValue g_auvs("nAuvs",
                              "Number of Auvs",
                              ns3::UintegerValue (1),
                              ns3::MakeUintegerChecker<uint32_t> ());
static ns3::GlobalValue g_auvModel("AuvModel",
                              "Auv Model",
                              ns3::UintegerValue (1),
                              ns3::MakeUintegerChecker<uint32_t> ());
static ns3::GlobalValue g_maxRange("MaxRange",
                              "Maximum range between gateway and acoustic node",
                              ns3::UintegerValue (1000),
                              ns3::MakeUintegerChecker<uint32_t> ());
static ns3::GlobalValue g_maxDepth("MaxDepth",
                              "Maximum depth between gateway and acoustic node",
                              ns3::UintegerValue (100),
                              ns3::MakeUintegerChecker<uint32_t> ());
static ns3::GlobalValue g_pktSize("PktSize",
                              "Packet size in bytes",
                              ns3::UintegerValue (32),
                              ns3::MakeUintegerChecker<uint32_t> ());
static ns3::GlobalValue g_simTime("SimTime",
                              "Simulation time per trial",
                               //Days(1),
                               ns3::DoubleValue (86400.01),
                               ns3::MakeDoubleChecker<double> ());
static ns3::GlobalValue g_simName("filename",
                              "Simulation name",
                              ns3::StringValue ("CampaignUan"),
                              ns3::MakeStringChecker());
static ns3::GlobalValue g_propModel("PropModel",
                              "Propagation Model",
                              ns3::StringValue ("UanPropModelIdeal"),
                              ns3::MakeStringChecker());
static ns3::GlobalValue g_noiseModel("NoiseModel",
                              "Noise Model",
                              ns3::StringValue ("UanNoiseModelDefault"),
                              ns3::MakeStringChecker());

static ns3::GlobalValue g_outputDir ("outputDir",
                                     "directory where to store simulation results",
                                     ns3::StringValue ("./"),
                                     ns3::MakeStringChecker ());

int
main (int argc, char *argv[])
{
  tic();
  Experiment exp;
  
  UintegerValue uintegervalue;
  DoubleValue doubleValue;
  EnumValue enumValue;
  BooleanValue booleanValue;
  StringValue stringvalue;

  CommandLine cmd;
  cmd.Parse (argc, argv);

  ConfigStore config;
  config.ConfigureDefaults ();
  config.ConfigureAttributes ();

    GlobalValue::GetValueByName ("nSinks", uintegervalue);
  uint32_t m_nsinks = uintegervalue.Get ();
    GlobalValue::GetValueByName ("nNodes", uintegervalue);
  uint32_t m_numNodes = uintegervalue.Get ();
    GlobalValue::GetValueByName ("nAuvs", uintegervalue);
  uint32_t m_auvs = uintegervalue.Get ();
    GlobalValue::GetValueByName ("AuvModel", uintegervalue);
  uint32_t m_auvModel = uintegervalue.Get ();
    GlobalValue::GetValueByName ("MaxRange", uintegervalue);
  uint32_t m_maxRange = uintegervalue.Get ();
    GlobalValue::GetValueByName ("MaxDepth", uintegervalue);
  uint32_t m_maxDepth = uintegervalue.Get ();
    GlobalValue::GetValueByName ("PktSize", uintegervalue);
  uint32_t m_pktSize = uintegervalue.Get ();
    GlobalValue::GetValueByName ("SimTime", doubleValue);
  double m_simTime = doubleValue.Get ();
    GlobalValue::GetValueByName ("filename", stringvalue);
  std::string m_filename = stringvalue.Get ();
    GlobalValue::GetValueByName ("NoiseModel", stringvalue);
  std::string m_noiseModel = stringvalue.Get ();
    GlobalValue::GetValueByName ("PropModel", stringvalue);
  std::string m_propModel = stringvalue.Get ();
    GlobalValue::GetValueByName ("outputDir", stringvalue);
  std::string m_outputdir = stringvalue.Get ();

  config.ConfigureAttributes ();
  exp.m_nsinks=m_nsinks;
  exp.m_numNodes=m_numNodes;
  exp.m_auvs=m_auvs;
  exp.m_auvModel=m_auvModel;
  exp.m_maxRange=m_maxRange;
  exp.m_maxDepth=m_maxDepth;
  exp.m_pktSize=m_pktSize;
  exp.m_simTime=Seconds(m_simTime);
  exp.m_filename = m_filename;
  exp.m_noiseModel=m_noiseModel;
  exp.m_propModel=m_propModel;
  exp.m_outputdir=m_outputdir;
 
  

  //TODO: Colocar o modelo do AUV usado na saida
  std::cout << "\nSIMULATION WITH PARAMETERS: " << std::endl;
  std::cout << "NumberNodes --: " << exp.m_numNodes << std::endl;
  std::cout << "NumberAUVs ---: " << exp.m_auvs << std::endl;
  std::cout << "AUVModel -----: " << exp.m_auvModel << std::endl;
  std::cout << "MaxRange -----: " << exp.m_maxRange << std::endl;
  std::cout << "MaxDepth -----: " << exp.m_maxDepth << std::endl;
  std::cout << "PktSize ------: " << exp.m_pktSize << std::endl;
  std::cout << "SimTime ------: " << exp.m_simTime << std::endl;
  std::cout << "PropModel ----: " << exp.m_propModel << std::endl;

  exp.Run (m_numNodes);

  toc();
}
