# Repositório do GppCom sobre investigações de Rede de Sensores Subaquáticas (do inglês Underwater Sensor Networks - UAN) e Veículos autônomos subaquáticos (do inglês Autonomous Underwater Vehicles - AUV) utilizando o simulador ns-3.

### Equipe
 - Idealizador/Orientador: Prof. Vicente Sousa (vicente.sousa@ufrn.edu.br)
 - Coordenador: MSc. Daniel Luna (daniel.luna.088@ufrn.edu.br)  
 - Equipe técnica: Daniel Luna (daniel.luna.088@ufrn.edu.br), Carlos Lima (carlos.lima.106@ufrn.edu.br) e Matheus Fagundes (matheus.fagundes.067@ufrn.edu.br).
 
### Primeiros passos:

O simulador de eventos discretos utilizado é o ns-3. Para cada versão do ns-3 utilizada pelo GppCom, há um arquivo patch necessário para integração das versões do ns-3 com o módulo AUV. Assim, basta apenas baixar o arquivo patch e aplicar na versão correta simulador.  

Downloads:

- Versão [ns-3.29](https://www.nsnam.org/releases/ns-allinone-3.29.tar.bz2) e [patch](https://github.com/vicentesousa/UANENCOM2021/blob/main/patches/auv-ns3-29.patch); 
- Versão [ns-3.30.1](https://www.nsnam.org/releases/ns-allinone-3.30.1.tar.bz2) e [patch](https://github.com/vicentesousa/UANENCOM2021/blob/main/patches/auv-ns3-30-1.patch);
- Versão [ns-3.31](https://www.nsnam.org/releases/ns-allinone-3.31.tar.bz2) e [patch](https://github.com/vicentesousa/UANENCOM2021/blob/main/patches/auv-ns3-31.patch);

Instruções:

- Os patchs foram criados com o comando: *diff -ruN ns-allinone-3.XX/ns-3.XX/ ns-allinone-3.XX_with_auv/ns-3.XX/ > auv-ns3-XX.patch*;
- O patch deve ser aplicado no diretório *ns-allinone-3.XX/ns-3.XX/*, por meio do comando: *patch -p1 -i auv-ns3-XX.patch*. Dessa forma, o patch deve estar localizado no diretório **ns-allinone-3.XX/**;


#### Exemplo desenvolvido pelo [Grupo de Pesquisa e Prototipagem em Soluções Rápidas para Comunicações (GppCom)](http://gppcom.ct.ufrn.br/)

O exemplo *uan-auv-aloha-ipv4.cc* é inspirado no *uan-energy-auv.cc*, originalmente desenvolvido para as primeiras versões do ns-3, porém foi descontinuado devido a não-inclusão oficial do módulo AUV no simulador. A descrição do exemplo pode ser encontrada na documentação oficial do simulador: [aqui](https://www.nsnam.org/docs/release/3.29/models/html/uan.html). E reproduzida abaixo:

*This is a comprehensive example where all the project’s components are used. We setup two nodes, one fixed surface gateway equipped with an acoustic modem and a moving Seaglider AUV with an acoustic modem too. Using the waypoint mobility model with an underlying GliderMobilityModel, we make the glider descend to -1000 meters and then emerge to the water surface. The AUV sends a generic 17-bytes packet every 10 seconds during the navigation process. The gateway receives the packets and stores the total bytes amount. At the end of the simulation are shown the energy consumptions of the two nodes and the networking stats.*

O exemplo desenvolvido pelo GppCom tem como objetivo reproduzir o exemplo original e adaptá-lo para uma versão do ns-3 mais recente, além de acrescentar as funcionalidades da camada IP ao script (função esta que foi disponibilizada ao módulo UAN apenas após a versão 3.28 do ns-3). O exemplo está disponível neste repositório com o nome de *uan-auv-ipv4.cc*.

#### Sobre 
- O *network simulator version 3*: [aqui](https://www.nsnam.org/);
- O módulo AUV do ns-3: [aqui](https://flore.unifi.it/retrieve/handle/2158/1087213/241093/AuvModels.pdf);
- Artigos: 
	- [Prototipando Veículos Autônomos Submarinos usando o ns-3](https://www.researchgate.net/publication/335777545_Prototipando_Veiculos_Autonomos_Submarinos_usando_o_ns-3)

## Caso de estudo do ENCOM 2021: Autonomous Underwater Vehicles 
- Nesse caso de estudo foi necessário realizar alterações no módulo AUV, segue o patch abaixo com as alterações:
- Versão [ns-3.31](https://www.nsnam.org/releases/ns-allinone-3.31.tar.bz2) e [patch](https://github.com/vicentesousa/UANENCOM2021/blob/main/patches/encom_ns3-31.patch);

### [Cenário 1]
**Objetivos**
 - Investigar o comportamento dos diferentes AUVs no cenário do exemplo base: [Prototipando Veículos Autônomos Submarinos usando o ns-3](https://www.researchgate.net/publication/335777545_Prototipando_Veiculos_Autonomos_Submarinos_usando_o_ns-3).
 - Resultados: https://github.com/vicentesousa/UANENCOM2021/tree/main/casos_de_estudo/cenario_01
 
### [Cenário 2]
**Objetivos**
 - Investigar o aumento do consumo de energia provocado pelo aumento deslocamento vertical.
 - Resultados: https://github.com/vicentesousa/UANENCOM2021/tree/main/casos_de_estudo/cenario_02
<!---
comentário
-->
