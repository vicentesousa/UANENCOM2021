#!/bin/bash
rm -rf /home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxRange/JOB49 2>/dev/null
mkdir -p /home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxRange/JOB49
cp -f run_Uan_campaign1_local_MaxRange_JOB49.sh /home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxRange
cp -f Uan_campaign1.yaml /home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxRange
cd '/home/c/ns-3/ns-allinone-3.31/ns-3.31/'

 ./waf --run 'uan-auv-ipv4-npad --RngRun=1540753736939065 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=10 --PktSize=29 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxRange/JOB49' --AuvModel=1 --MaxRange=10' &
wait

 ./waf --run 'uan-auv-ipv4-npad --RngRun=1540753736939065 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=10 --PktSize=29 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxRange/JOB49' --AuvModel=1 --MaxRange=1000' &
wait

 ./waf --run 'uan-auv-ipv4-npad --RngRun=1540753736939065 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=10 --PktSize=29 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxRange/JOB49' --AuvModel=1 --MaxRange=2000' &
wait

 ./waf --run 'uan-auv-ipv4-npad --RngRun=1540753736939065 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=10 --PktSize=29 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxRange/JOB49' --AuvModel=1 --MaxRange=3000' &
wait

 ./waf --run 'uan-auv-ipv4-npad --RngRun=1540753736939065 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=10 --PktSize=29 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxRange/JOB49' --AuvModel=1 --MaxRange=4000' &
wait

 ./waf --run 'uan-auv-ipv4-npad --RngRun=1540753736939065 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=10 --PktSize=29 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxRange/JOB49' --AuvModel=1 --MaxRange=5000' &
wait

 ./waf --run 'uan-auv-ipv4-npad --RngRun=1540753736939065 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=10 --PktSize=29 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxRange/JOB49' --AuvModel=2 --MaxRange=10' &
wait

 ./waf --run 'uan-auv-ipv4-npad --RngRun=1540753736939065 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=10 --PktSize=29 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxRange/JOB49' --AuvModel=2 --MaxRange=1000' &
wait

 ./waf --run 'uan-auv-ipv4-npad --RngRun=1540753736939065 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=10 --PktSize=29 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxRange/JOB49' --AuvModel=2 --MaxRange=2000' &
wait

 ./waf --run 'uan-auv-ipv4-npad --RngRun=1540753736939065 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=10 --PktSize=29 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxRange/JOB49' --AuvModel=2 --MaxRange=3000' &
wait

 ./waf --run 'uan-auv-ipv4-npad --RngRun=1540753736939065 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=10 --PktSize=29 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxRange/JOB49' --AuvModel=2 --MaxRange=4000' &
wait

 ./waf --run 'uan-auv-ipv4-npad --RngRun=1540753736939065 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=10 --PktSize=29 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxRange/JOB49' --AuvModel=2 --MaxRange=5000' &
wait

 ./waf --run 'uan-auv-ipv4-npad --RngRun=1540753736939065 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=10 --PktSize=29 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxRange/JOB49' --AuvModel=3 --MaxRange=10' &
wait

 ./waf --run 'uan-auv-ipv4-npad --RngRun=1540753736939065 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=10 --PktSize=29 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxRange/JOB49' --AuvModel=3 --MaxRange=1000' &
wait

 ./waf --run 'uan-auv-ipv4-npad --RngRun=1540753736939065 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=10 --PktSize=29 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxRange/JOB49' --AuvModel=3 --MaxRange=2000' &
wait

 ./waf --run 'uan-auv-ipv4-npad --RngRun=1540753736939065 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=10 --PktSize=29 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxRange/JOB49' --AuvModel=3 --MaxRange=3000' &
wait

 ./waf --run 'uan-auv-ipv4-npad --RngRun=1540753736939065 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=10 --PktSize=29 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxRange/JOB49' --AuvModel=3 --MaxRange=4000' &
wait

 ./waf --run 'uan-auv-ipv4-npad --RngRun=1540753736939065 --SimTime=86400 --nNodes=0 --nSinks=1 --nAuvs=1 --AuvModel=1 --MaxRange=10 --PktSize=29 --NoiseModel=UanNoiseModelDefault --PropModel=UanPropModelThorp --filename=CampaignUan --outputDir='/home/c/ns-3/ns-allinone-3.31/ns-3.31/results_local_Uan_campaign1_MaxRange/JOB49' --AuvModel=3 --MaxRange=5000' &
wait
