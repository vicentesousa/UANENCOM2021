import warnings
warnings.filterwarnings("ignore")
import numpy as np
import matplotlib.pylab as plt
from matplotlib.ticker import (MultipleLocator, 
                               FormatStrFormatter, 
                               AutoMinorLocator)
from itertools import cycle
import scipy.stats as st
import os
import argparse
import yaml
import matplotlib
from random import randint   
import itertools  


class openSimulation:
    def __init__(self, configurations_file):
        self.email_to = 'fulano@gmail.com'            
        with open(configurations_file, 'r') as f:
            self.doc = yaml.load(f, Loader=yaml.loader.BaseLoader)
            #self.doc = doc
        self.campaign_name = os.path.splitext(configurations_file)[0]
        
        # Shell Script Parameters configuration
        self.nOfCurlines=self.doc['scenario']['nOfCurlines']
        self.SimTied = self.doc['scenario']['SimTied']
        self.nOfCurlinesTied = self.doc['scenario']['nOfCurlinesTied']
        # ns-3 script configuration
        self.ns3_path = str(self.doc['scenario']['ns3_path'])
        self.ns3_path = os.getcwd() + '/' + self.ns3_path
        self.plot_path = os.getcwd()
        self.ns3_script = str(self.doc['scenario']['ns3_script'])
        self.filename = str(self.doc['scenario']['filename'])
        self.simLocation = str(doc['scenario']['simLocation'])
        
        self.showPlot = True;
        self.plotCI = False;                
                
        # Read simulation parameters
        self.campaignX = self.doc['scenario']['campaignX']
        self.campaignLines = self.doc['scenario']['campaignLines']
        self.nJobs = int(self.doc['scenario']['jobs'])
        if self.nJobs == 1: # disable the confidence interval
            self.plotCI = False
        self.SimTime = self.doc['scenario']['SimTime']
        self.nSinks = self.doc['scenario']['nSinks']
        self.nNodes = self.doc['scenario']['nNodes']
        self.nAuvs = self.doc['scenario']['nAuvs']
        self.AuvModel = self.doc['scenario']['AuvModel']
        self.AuvModel = list(map(int,self.AuvModel))
        self.MaxRange = self.doc['scenario']['MaxRange']
        self.MaxRange = list(map(int,self.MaxRange))
        self.MaxDepth = self.doc['scenario']['MaxDepth']
        self.MaxDepth = list(map(int,self.MaxDepth))
        self.PktSize = self.doc['scenario']['PktSize']
        self.NoiseModel = self.doc['scenario']['NoiseModel']
        self.PropModel = self.doc['scenario']['PropModel']
       
                 
        #self.fixedSeed = (self.doc['scenario']['fixedSeed'])
        
    def plotCampaign(self,curCampaign, metric, plotgraph):
        # some general configurations from .txt file
        outputDir = self.plot_path
        m_metric,mm_metric1,mm_metric2,mm_metric3,mm_metric4=[],[],[],[],[]
        m_metricFlow1,m_metricFlow2,mm_metricFlow1,mm_metricFlow2 = [],[],[],[]
        m_metricCI1,m_metricCI2,m_metricCI3,m_metricCI4=[],[],[],[]
        m_MetricFlow1CI,m_MetricFlow2CI = [],[]
        markers_on_all = cycle(list(itertools.product([0], [1,2,3,4,5])))
        markersA = cycle(('o', 'v', 'D', '>', 'h', '^', '*', '>', 's', 'H', 'D', 'd'))
        colors = cycle(('b', 'g', 'r', 'c', 'm', 'y', 'k'))                    
        plt.figure()     
        nOfCurlines=int(self.nOfCurlines[0]) # number of Campaign Lines in 1 simulation (max value = 3)
        SimTied = int(self.SimTied[0]) # whether or not the simulation parameters are tied (0 or 1)
        nOfCurlinesTied = int(self.nOfCurlinesTied[0]) # number of Campaign Lines tied (max value = nOfCurlines)
        print (nOfCurlines, SimTied,nOfCurlinesTied)
        if nOfCurlines == 1 and SimTied == 0:
            curLine = self.doc['scenario']['campaignLines'][0]
            #curLinePlus = self.doc['scenario']['campaignLines'][nOfCurlines-nOfCurlinesTied+1]
            curves = len(self.doc['scenario'][curLine])  #used for differentiate plot
            campaignX = self.campaignX[0]
            color=next(colors)
            marker=next(markersA)
            markers_on=next(markers_on_all)
            #self.campaignX
            for iCampaignX in range(len(self.doc['scenario'][campaignX])):
                for i in range(len(self.doc['scenario'][curLine])):
                    #for j in range(len(self.doc['scenario'][curLinePlus])):
                    if plotgraph == 'MaxDepth':
                        print(self.MaxDepth)
                        curFileName = ("UL_Results_Sim_PropModel_UanPropModelThorp"+
                                   "_MaxDepth_"+str(int(self.MaxDepth[iCampaignX]))+
                                   "_AuvModel_"+str(int(self.AuvModel[i]))+".txt")
                        
                    for iJob in range(0,self.nJobs):
                        CurSimuFile = outputDir + "/JOB" +str(iJob) + "/" + curFileName
                        #print(CurSimuFile)
                        Rx_Packets = np.loadtxt(CurSimuFile, skiprows=1, usecols=(5, ), delimiter=',', unpack=False)
                        #m_pkt = np.append(m_pkt, np.sum(Rx_Packets))
                        T_put = np.loadtxt(CurSimuFile, skiprows=1, usecols=(7, ), delimiter=',', unpack=False)
                        #m_tput = np.append(m_tput, np.sum(Tput))
                        Packet_Loss_Ratio = np.loadtxt(CurSimuFile, skiprows=1, usecols=(10, ), delimiter=',', unpack=False)
                        #m_plr = np.append(m_plr, np.sum(Packet_Loss_Ratio))
                        Modem_Consumption = np.loadtxt(CurSimuFile, skiprows=1, usecols=(12, ), delimiter=',', unpack=False)
                        #m_navModemConsumption = np.append(m_navModemConsumption, np.sum(Nav_Modem_Consumption))
                        Nav_Consumption = np.loadtxt(CurSimuFile, skiprows=1, usecols=(11, ), delimiter=',', unpack=False)
                        #m_navConsumption = np.append(m_navConsumption, np.sum(Nav_Consumption))
                        
                        if metric == 'PLR':
                            ylabel="Taxa de Perda de Pacotes"
                            m_metric = np.append(m_metric, np.sum(Packet_Loss_Ratio))   
                        elif metric == 'T_put':
                            ylabel="Taxa Útil [bps]"
                            m_metric = np.append(m_metric, np.sum(T_put*1000000))
                        elif metric == 'Pkt':
                            ylabel= "Pacotes Recebidos"
                            m_metric = np.append(m_metric, np.sum(Rx_Packets))
                        elif metric == 'ModemConsumption':
                            ylabel= "Energia Consumida [J]"
                            m_metric = np.append(m_metric, np.sum(Modem_Consumption))    
                        elif metric == 'NavConsumption':
                            ylabel= "Energia Consumida [J]"
                            m_metric = np.append(m_metric, np.sum(Nav_Consumption))
                        elif metric == 'NavModemConsumption':
                            ylabel= "Energia Consumida [kJ]"
                            m_metric = np.append(m_metric, np.sum((Modem_Consumption + Nav_Consumption)/1000))

                            
                        
                    #Calculate CI
                    l, hmetric = st.t.interval(0.95, len(m_metric), loc=np.mean(m_metric), scale=st.sem(m_metric))
                    #l, hplr = st.t.interval(0.95, len(m_plr), loc=np.mean(m_plr), scale=st.sem(m_plr))
                    #l, htput = st.t.interval(0.95, len(m_tput), loc=np.mean(m_tput), scale=st.sem(m_tput))
 
                    if self.AuvModel[i] == 1: #Glider 
                        mm_metric1 = np.append(mm_metric1, np.mean(m_metric))
                        l, hmetric1 = st.t.interval(0.95, len(m_metric), loc=np.mean(m_metric), scale=st.sem(m_metric))                           
                        m_metricCI1 = np.append(m_metricCI1, hmetric1)
                    
                    elif self.AuvModel[i] == 2: #Remus
                        mm_metric2 = np.append(mm_metric2, np.mean(m_metric))
                        l, hmetric2 = st.t.interval(0.95, len(m_metric), loc=np.mean(m_metric), scale=st.sem(m_metric))                           
                        m_metricCI2 = np.append(m_metricCI2, hmetric2)
                        
                    elif self.AuvModel[i] == 3: #Typhoon
                        mm_metric3 = np.append(mm_metric3, np.mean(m_metric))
                        l, hmetric3 = st.t.interval(0.95, len(m_metric), loc=np.mean(m_metric), scale=st.sem(m_metric))                           
                        m_metricCI3 = np.append(m_metricCI3, hmetric3)
                     
                    m_metric=[]
                    
            if plotgraph == 'MaxDepth':
                resxDataold = eval('sorted(self.'+curCampaign+',key=int)')
                resxData = np.array(resxDataold)
                #Labels
                xlabel='Profundidade do AUV [m]'
                label1 = "Glider"
                label2 = "Remus"
                label3 = "Typhoon"
         
            
            # Plot figure
            params = {'legend.fontsize': 'x-large',
             'axes.labelsize': 'x-large',
             'axes.titlesize':'x-large',
             'xtick.labelsize':'x-large',
             'ytick.labelsize':'x-large'}
            plt.rcParams.update(params)        
            os.makedirs(outputDir+"/ps", exist_ok=True)
            os.makedirs(outputDir+"/png", exist_ok=True)
            print()
        # Confidence interval according to https://stackoverflow.com/questions/15033511/compute-a-confidence-interval-from-sample-data
        if plotgraph == 'MaxDepth':
            if bool(self.plotCI):  
                if metric == 'NavConsumption':
                    fig = plt.figure() 
                    ax=fig.add_subplot(111)
                    plt.yscale('log')
                    ax.set_yticks([20, 100, 500,2500 ,10000 ,50000 ,165000])
                    ax.get_yaxis().set_major_formatter(matplotlib.ticker.ScalarFormatter())

                    ax.plot(resxData,mm_metric1,label=label1, marker=marker,color=color,markevery=markers_on)
                    ax.errorbar(resxData,mm_metric1, yerr = (m_metricCI1-mm_metric1), marker=marker,color=color, ls = 'none', lw = 2, capthick = 2,markevery=markers_on)
                    color=next(colors)
                    marker=next(markersA)
                    markers_on=next(markers_on_all)

                    ax.plot(resxData,mm_metric2,label=label2, marker=marker,color=color,markevery=markers_on)
                    ax.errorbar(resxData,mm_metric2, yerr = (m_metricCI2-mm_metric2), marker=marker,color=color, ls = 'none', lw = 2, capthick = 2,markevery=markers_on)
                    color=next(colors)
                    marker=next(markersA)
                    markers_on=next(markers_on_all)

                    ax.plot(resxData,mm_metric3,label=label3, marker=marker,color=color,markevery=markers_on)
                    ax.errorbar(resxData,mm_metric3, yerr = (m_metricCI3-mm_metric3), marker=marker,color=color, ls = 'none', lw = 2, capthick = 2,markevery=markers_on)
                    color=next(colors)
                    marker=next(markersA)
                    markers_on=next(markers_on_all)
                   
                else:
                    plt.plot(resxData,mm_metric1, label=label1, marker=marker,color=color,markevery=markers_on)
                    plt.errorbar(resxData,mm_metric1, yerr = (m_metricCI1-mm_metric1), marker=marker,color=color, ls = 'none', lw = 2, capthick = 2,markevery=markers_on)
                    color=next(colors)
                    marker=next(markersA)
                    markers_on=next(markers_on_all)
                    
                    plt.plot(resxData,mm_metric2, label=label2, marker=marker,color=color,markevery=markers_on)
                    plt.errorbar(resxData,mm_metric2, yerr = (m_metricCI2-mm_metric2), marker=marker,color=color, ls = 'none', lw = 2, capthick = 2,markevery=markers_on)
                    color=next(colors)
                    marker=next(markersA)
                    markers_on=next(markers_on_all)
                    
                    plt.plot(resxData,mm_metric3, label=label3, marker=marker,color=color,markevery=markers_on)
                    plt.errorbar(resxData,mm_metric3, yerr = (m_metricCI3-mm_metric3), marker=marker,color=color, ls = 'none', lw = 2, capthick = 2,markevery=markers_on)
                    color=next(colors)
                    marker=next(markersA)
                    markers_on=next(markers_on_all)
                    
            else:
                if metric == 'NavConsumption':
                    fig = plt.figure() 
                    ax=fig.add_subplot(111)
                    plt.yscale('log')
                    ax.set_yticks([20, 100, 500,2500 ,10000 ,50000 ,165000])
                    ax.get_yaxis().set_major_formatter(matplotlib.ticker.ScalarFormatter())

                    ax.plot(resxData,mm_metric1,label=label1, marker=marker,color=color,markevery=markers_on)
                    color=next(colors)
                    marker=next(markersA)
                    markers_on=next(markers_on_all)

                    ax.plot(resxData,mm_metric2,label=label2, marker=marker,color=color,markevery=markers_on)
                    color=next(colors)
                    marker=next(markersA)
                    markers_on=next(markers_on_all)

                    ax.plot(resxData,mm_metric3,label=label3, marker=marker,color=color,markevery=markers_on)
                    color=next(colors)
                    marker=next(markersA)
                    markers_on=next(markers_on_all)
                else:
                    plt.plot(resxData,mm_metric1, label=label1, marker=marker,color=color,markevery=markers_on)
                    color=next(colors)
                    marker=next(markersA)
                    markers_on=next(markers_on_all)
                    
                    plt.plot(resxData,mm_metric2, label=label2, marker=marker,color=color,markevery=markers_on)  
                    color=next(colors)
                    marker=next(markersA)
                    markers_on=next(markers_on_all)
                    
                    plt.plot(resxData,mm_metric3, label=label3, marker=marker,color=color,markevery=markers_on)
                    color=next(colors)
                    marker=next(markersA)
                    markers_on=next(markers_on_all)

            print("Glider:", mm_metric1)
            print("Remus:", mm_metric2)
            print("Typhoon:", mm_metric3)

            plt.xlabel(xlabel)
            plt.ylabel(ylabel)
            #plt.xlim([0.10, 0.55]) 
            #plt.legend(loc='best', numpoints=1)
            #plt.legend(loc='center left', bbox_to_anchor=(0.2, 0.13))
            if metric == 'T_put':
                plt.legend(loc='center left', bbox_to_anchor=(0.1, 0.2))
                plt.axis([8,102,0, 20])            
            elif metric == 'PLR':
                plt.legend(loc='center left', bbox_to_anchor=(0.1, 0.25))
                #plt.axis([8,102,0.264, 0.271])
                plt.axis([8,102,0, 0.3])
            elif metric == 'ModemConsumption':
                plt.legend(loc='center left', bbox_to_anchor=(0.025, 0.2))
                plt.ticklabel_format(style='plain')
                plt.axis([8,102,0, 1400000])
            elif metric == 'NavConsumption':
                plt.legend(loc='center left', bbox_to_anchor=(0.1, 0.5))
                plt.ticklabel_format(style='plain')
            elif metric == 'NavModemConsumption':
                plt.legend(loc='center left', bbox_to_anchor=(0.025, 0.2))
                plt.ticklabel_format(style='plain')
                plt.axis([8,102,0, 1600])
            plt.grid()
            plt.tight_layout()
            #plt.show()
        
            
        #Save PlotCI
        if bool(self.plotCI):
            imgfilename = str(metric)+'_CI_'+curCampaign
        else:
            imgfilename = str(metric)+'_'+curCampaign    
        plt.savefig(outputDir+"/png/"+imgfilename+".png")
        plt.savefig(outputDir+"/ps/"+imgfilename+".eps")       
        if bool(self.showPlot):
            plt.show()
        else:
            plt.close()
                  



parser = argparse.ArgumentParser()
#parser = "CaNR_campaign.yaml"
parser.add_argument("-f", "--file", type=str, help='Configuration File')
parser.add_argument("-g", "--graph", type=str, help='Type of Graph (Bitrate,Rcc,UePos) supported')
parser.add_argument("-m", "--metric", type=str, help='List of metrics (PLR, Tput) desired. Can be one or more.')
parser.add_argument("-p", "--path", type=str, help='Path')
args = parser.parse_args()
configurations_file = args.file;
plotgraph = args.graph;
plotpath = args.path;
metrics = args.metric.split(',')
os.chdir(plotpath)
 
with open(configurations_file, 'r') as f:
    doc = yaml.load(f, Loader=yaml.loader.BaseLoader)
    campaign_name = os.path.splitext(configurations_file)[0]
    print('Simulação escolhida: ')

"""
#For debug purposes:
plotpath = "/media/daniel/SharedDisk/Clouds/Insync/SharedWithMe/UFRN/GITHUB/ns3-nr/Campaigns/results_cluster_NrCa_Scenario3gpp_SimPerScript_campaign6_UePosY"
os.chdir(plotpath)

doc = {'scenario': {'nOfCurlines': ['3'],
  'SimTied': ['1'],
  'nOfCurlinesTied': ['2'],
  'daytime': ['1'],
  'htime': ['0'],
  'ntasks': ['16'],
  'ns3_path': '',
  'ns3_script': 'NrCa-Scenario3gpp',
  'cluster_path': '/home/drdluna/ns-3-dev-nr/',
  'filename': 'CampaignNrCa',
  'simLocation': 'cluster',
  'campaignX': ['UePosY'],
  'campaignLines': ['UseCa', 'bandwidthBwp1', 'UseBlockage'],
  'jobs': '10',
  'SimTime': '5',
  'frequencyBwp1': ['24000000000'],
  'bandwidthBwp1': ['800000000', '400000000'],
  'frequencyBwp2': ['24400000000'],
  'bandwidthBwp2': ['400000000'],
  'BitRate': ['3500000000'],
  'UseCa': ['0', '1'],
  'UseBlockage': ['0', '1'],
  'UePosY': ['1', '10', '50', '100']}}

configurations_file = "NrCa_Scenario3gpp_SimPerScript_campaign6.yaml"
plotgraph= 'UePosY'
"""
campaign = doc['scenario']['campaignX']
simCampaign = str(campaign[0])

print(campaign)
simu = openSimulation(configurations_file)

for iMet in metrics:
    simu.plotCampaign(simCampaign,iMet,plotgraph);
        
         #   print('Invalid simulation campaign: verify the campaign parameter!')
