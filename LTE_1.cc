/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Manuel Requena <manuel.requena@cttc.es>
 */

#include "ns3/config-store-module.h"
#include "ns3/core-module.h"
#include "ns3/lte-module.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "string.h"
#include <iostream>

using namespace ns3;

using namespace std;

int
main(int argc, char* argv[])
{

    Time simTime = Seconds(5);
    double interSiteDistance = 10000;
    double c = 400;
    double numberOfEnbs = 3;    
    int schedType = 3;
    
    // Command line arguments
    CommandLine cmd(__FILE__);
    cmd.AddValue("scheduler", "type of scheduler to use with the network devices", schedType);
    cmd.AddValue("simTime", "Total duration of the simulation (in seconds)", simTime);
    cmd.AddValue("interSiteDistance", "Inter-site distance in meter", interSiteDistance);
    cmd.AddValue("c", "First distance to base station", c);
    cmd.AddValue("numberOfEnbs", "Number of eNBsz", numberOfEnbs);
    //cmd.AddValue("numberOfEnbs2", "Number of eNBs", numberOfEnbs2);

    // Config::SetDefault("ns3::LteUePhy::TxPower", DoubleValue(40.0));
   
    cmd.Parse(argc, argv);

    ConfigStore inputConfig;
    inputConfig.ConfigureDefaults();

    // double eNbTxPower = 30;
    // Config::SetDefault ("ns3::LteEnbPhy::TxPower", DoubleValue (eNbTxPower));
    // Config::SetDefault ("ns3::LteUePhy::TxPower", DoubleValue (1.0));
    // Config::SetDefault ("ns3::LteUePhy::EnableUplinkPowerControl", BooleanValue (true));

    // Config::SetDefault ("ns3::LteUePowerControl::ClosedLoop", BooleanValue (true));
    // Config::SetDefault ("ns3::LteUePowerControl::AccumulationEnabled", BooleanValue (true));
    // Config::SetDefault ("ns3::LteUePowerControl::Alpha", DoubleValue (1.0));

    // Config::SetDefault("ns3::LteHelper::UseCa", BooleanValue(true));
    // Config::SetDefault("ns3::LteHelper::NumberOfComponentCarriers", UintegerValue(5));
    // Config::SetDefault("ns3::LteHelper::EnbComponentCarrierManager", StringValue("ns3::RrComponentCarrierManager"));
    
    Ptr<LteHelper> lteHelper = CreateObject<LteHelper>();
    switch(schedType) {
        case 0:
        	lteHelper->SetSchedulerType ("ns3::RrFfMacScheduler"); //!!!  round robin
       	break;
        case 1:
		lteHelper->SetSchedulerType ("ns3::PssFfMacScheduler"); // priority set
		//@@@ lteHelper->SetSchedulerAttribute("nMux", UIntegerValue(numberOfEnbs)); // the maximum number of UE selected by TD scheduler
		lteHelper->SetSchedulerAttribute("PssFdSchedulerType", StringValue("CoItA")); // PF scheduler type in PSS
		break;
	case 2:
		lteHelper->SetSchedulerType ("ns3::PfFfMacScheduler"); //  proportional fair
        	break;
   	case 3:
        	lteHelper->SetSchedulerType ("ns3::TdMtFfMacScheduler"); //max td
       	break;
	case 4:
		lteHelper->SetSchedulerType ("ns3::FdMtFfMacScheduler"); //max fd
		break;
	case 5:
		lteHelper->SetSchedulerType ("ns3::TtaFfMacScheduler"); //tta
		break;
	case 6:
		lteHelper->SetSchedulerType("ns3::FdBetFfMacScheduler");   // FD-BET scheduler
		break;
	case 7:
		lteHelper->SetSchedulerType("ns3::TdBetFfMacScheduler");   // TD-BET scheduler
		break;
	case 8:
		lteHelper->SetSchedulerType("ns3::FdTbfqFfMacScheduler");  // FD-TBFQ scheduler
		break;
	case 9:
		lteHelper->SetSchedulerType("ns3::TdTbfqFfMacScheduler");  // TD-TBFQ scheduler
		break;
	case 10:
		lteHelper->SetSchedulerType("ns3::CqaFfMacScheduler");  // Channel and QoS Aware Scheduler
		break;	
	default:
		lteHelper->SetSchedulerType("ns3::RrFfMacScheduler"); 
    }

    lteHelper->SetAttribute("PathlossModel", StringValue("ns3::FriisSpectrumPropagationLossModel"));
    
    // Enable LTE log components
    // lteHelper->EnableLogComponents ();

    // Create Nodes: eNodeB and UE
    NodeContainer enbNodes;
    NodeContainer ueNodes;
    enbNodes.Create(1);
    ueNodes.Create(numberOfEnbs);

    // Install Mobility Model
    MobilityHelper mobility;
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(enbNodes);
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(ueNodes);

    

    // Create Devices and install them in the Nodes (eNB and UE)
    NetDeviceContainer enbDevs;
    NetDeviceContainer ueDevs;
    enbDevs = lteHelper->InstallEnbDevice(enbNodes);
    //printf(enbDevs->GetSchedulerType());
    ueDevs = lteHelper->InstallUeDevice(ueNodes);

    // Attach a UE to a eNB
    lteHelper->Attach(ueDevs, enbDevs.Get(0));

    // Activate an EPS bearer
    enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
    EpsBearer bearer(q);
    lteHelper->ActivateDataRadioBearer(ueDevs, bearer);

    Simulator::Stop(simTime);

    lteHelper->EnablePhyTraces();
    lteHelper->EnableMacTraces();
    lteHelper->EnableRlcTraces();

    std::vector<double> userDistance;
    
    // double co = 400;
    for (int i = 0; i < numberOfEnbs; i++)
    {
        cout << c << endl;
        userDistance.push_back(c);
        c+=interSiteDistance;
        Ptr<ConstantPositionMobilityModel> mm = ueNodes.Get(i)->GetObject<ConstantPositionMobilityModel>();
        mm->SetPosition(Vector(userDistance[i], 0.0, 0.0));
    }

    Simulator::Run();

    Simulator::Destroy();

    return 0;
}
