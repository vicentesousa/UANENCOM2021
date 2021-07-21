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

#ifndef UAN_RC_EXAMPLE_H
#define UAN_RC_EXAMPLE_H

#include "ns3/network-module.h"
#include "ns3/stats-module.h"
#include "ns3/uan-module.h"
//#include "ns3/config-store-module.h"


using namespace ns3;

/**
 * \ingroup uan
 *
 * Container for the parameters describing a single experiment.
 *
 * An experiment samples a range of parameter values.  The parameter
 * controls the number of nodes (if m_doNode is true), or the
 * UanMacRcGw MaxReservations attribute.
 *
 * \see uan-rc-example.cc
 *
 */
/*static ns3::GlobalValue g_nSinks ("VRCnSinks",
                                  "Number of sink nodes for routing non-BSM traffic",
                                  ns3::UintegerValue (1),
                                  ns3::MakeUintegerChecker<uint32_t> ());

g_nSinks.SetValue (UintegerValue (m_nSinks));

static ns3::GlobalValue g_numNodes ("Nodes",
                                  "Number of sink nodes for routing non-BSM traffic",
                                  ns3::UintegerValue (1),
                                  ns3::MakeUintegerChecker<uint32_t> ())  ;

g_numNodes.SetValue (UintegerValue (m_numNodes));*/

class Experiment
{
public:
  uint32_t m_nsinks;
  uint32_t m_numNodes;   //!< Number of fixed nodes (invalid for m_doNode true).
  uint32_t m_auvs;       //!< Number of auvs (invalid for m_doNode true).
  uint32_t m_auvModel;   //!< Auv model (1-Glider, 2-Remus, 3-Typhoon)
  uint32_t m_maxRange;   //!< Maximum range between gateway and acoustic node.
  uint32_t m_pktSize;    //!< Packet size in bytes.
  Time m_simTime;        //!< Simulation time per trial

  std::string m_filename;
  std::string m_propModel;
  std::string m_noiseModel;
  std::string m_outputdir;    //!< Filename for GnuPlot.

  //ConfigStore config;

  uint32_t m_bytesTotal; //!< Total number of bytes received in a simulation run.

  /**
   * Callback to receive a packet.
   *
   * \param socket The socket receiving packets.
   */
  void ReceivePacket (Ptr<Socket> socket);
  /**
   * Create a UanTxMode.
   *
   * The mode physical rate is set equal to m_totalRate.
   * The data rate and bandwidth are set to
   *
   * \f[{\rm{rate, bandwidth}} = {\rm{kass}}\frac{{{\rm{m\_totalRate}}}}{{{\rm{m\_numRates}} + 1}}\f]
   *
   * The center frequency is set to
   *
   * \f[{f_{center}} = {\rm{fc}} + \frac{{ \pm {\rm{m\_totalRate}} \mp {\rm{rate}}}}{2}\f]
   *
   * where the upper sign is taken if upperblock is true.
   *
   * \param kass Fraction of total bandwidth assigned to mode.
   * \param fc Mode center frequency offset.
   * \param upperblock Sign choice in setting the center frequency.
   * \param name Mode name.
   * \return The new mode.
   */
    /**
   * Create m_numRates matching control and data modes.
   *
   * \param fc Mode center frequency offset.
   */
  //void CreateDualModes (uint32_t fc);
  /**
   * Run a parametrized experiment.
   *
   * The parameter sets either the number of nodes (if m_doNode is true)
   * or the \"a\" parameter, which controls the UanMacRcGw MaxReservations
   * attribute.
   *
   * \param param The parameter value.
   * \return The total number of bytes delivered.
   */
  uint32_t Run (uint32_t param);

  void PrintReceivedPacket (Ptr<Socket> socket);

  void PrintReceivedPacketAuv (Ptr<Socket> socket);


  void SendPackets ();

  /**
   * Send a packet from one of the nodes
   * \param node The sending node
   * \param pkt The packet
   * \param dst the destination
   */
  void SendSinglePacket (Ptr<Node> node, Ptr<Packet> pkt, Ipv4Address dst,Ipv4Address orgn);

  /**
   * Send a packet from one of the auvs
   * \param node The sending node
   * \param pkt The packet
   * \param dst the destination
   */
  void SendSinglePacketAuv (Ptr<Node> node, Ptr<Packet> pkt, Ipv4Address dst,Ipv4Address orgn);

  /** Default constructor. */
  Experiment();

private:
  	NodeContainer nodes; //!< UAN nodes
    std::map<Ptr<Node>, Ptr<Socket> > m_sockets; //!< send and receive sockets
    std::map<Ptr<Node>, Ptr<Socket> > m_sockets_sink; //!< send and receive sockets
    std::map<Ptr<Node>, Ptr<Socket> > m_sockets_auv; //!< send and receive sockets
};

#endif /* UAN_RC_EXAMPLE_H */
