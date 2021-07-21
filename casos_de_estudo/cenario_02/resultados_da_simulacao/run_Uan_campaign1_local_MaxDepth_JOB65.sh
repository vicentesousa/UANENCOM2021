#!/bin/bash
rm -rf /home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxDepth/JOB65 2>/dev/null
mkdir -p /home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxDepth/JOB65
cp -f run_Uan_campaign1_local_MaxDepth_JOB65.sh /home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxDepth
cp -f Uan_campaign1.yaml /home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxDepth
cd '/home/c/ns-3/ns-allinone-3.31/ns-3.31/'
 ./waf --run 'uan-auv-ipv4-npad --RngRun=1681887051744379 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=1000 --PktSize=32 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxDepth/JOB65' --AuvModel=1 --MaxDepth=10' &
wait
 ./waf --run 'uan-auv-ipv4-npad --RngRun=1681887051744379 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=1000 --PktSize=32 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxDepth/JOB65' --AuvModel=1 --MaxDepth=25' &
wait
 ./waf --run 'uan-auv-ipv4-npad --RngRun=1681887051744379 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=1000 --PktSize=32 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxDepth/JOB65' --AuvModel=1 --MaxDepth=50' &
wait
 ./waf --run 'uan-auv-ipv4-npad --RngRun=1681887051744379 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=1000 --PktSize=32 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxDepth/JOB65' --AuvModel=1 --MaxDepth=75' &
wait
 ./waf --run 'uan-auv-ipv4-npad --RngRun=1681887051744379 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=1000 --PktSize=32 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxDepth/JOB65' --AuvModel=1 --MaxDepth=100' &
wait
 ./waf --run 'uan-auv-ipv4-npad --RngRun=1681887051744379 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=1000 --PktSize=32 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxDepth/JOB65' --AuvModel=2 --MaxDepth=10' &
wait
 ./waf --run 'uan-auv-ipv4-npad --RngRun=1681887051744379 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=1000 --PktSize=32 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxDepth/JOB65' --AuvModel=2 --MaxDepth=25' &
wait
 ./waf --run 'uan-auv-ipv4-npad --RngRun=1681887051744379 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=1000 --PktSize=32 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxDepth/JOB65' --AuvModel=2 --MaxDepth=50' &
wait
 ./waf --run 'uan-auv-ipv4-npad --RngRun=1681887051744379 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=1000 --PktSize=32 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxDepth/JOB65' --AuvModel=2 --MaxDepth=75' &
wait
 ./waf --run 'uan-auv-ipv4-npad --RngRun=1681887051744379 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=1000 --PktSize=32 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxDepth/JOB65' --AuvModel=2 --MaxDepth=100' &
wait
 ./waf --run 'uan-auv-ipv4-npad --RngRun=1681887051744379 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=1000 --PktSize=32 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxDepth/JOB65' --AuvModel=3 --MaxDepth=10' &
wait
 ./waf --run 'uan-auv-ipv4-npad --RngRun=1681887051744379 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=1000 --PktSize=32 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxDepth/JOB65' --AuvModel=3 --MaxDepth=25' &
wait
 ./waf --run 'uan-auv-ipv4-npad --RngRun=1681887051744379 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=1000 --PktSize=32 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxDepth/JOB65' --AuvModel=3 --MaxDepth=50' &
wait
 ./waf --run 'uan-auv-ipv4-npad --RngRun=1681887051744379 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=1000 --PktSize=32 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxDepth/JOB65' --AuvModel=3 --MaxDepth=75' &
wait
 ./waf --run 'uan-auv-ipv4-npad --RngRun=1681887051744379 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=1000 --PktSize=32 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxDepth/JOB65' --AuvModel=3 --MaxDepth=100' &
wait
