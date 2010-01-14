<map version="0.8.1">
<!-- To view this file, download free mind mapping software FreeMind from http://freemind.sourceforge.net -->
<node COLOR="#000000" CREATED="1238579981247" ID="Freemind_Link_602694754" MODIFIED="1238588716836" STYLE="bubble" TEXT="NRTB">
<font NAME="SansSerif" SIZE="20"/>
<hook NAME="accessories/plugins/AutomaticLayout.properties"/>
<node COLOR="#0033ff" CREATED="1238580018246" ID="_" MODIFIED="1238587063294" POSITION="right" STYLE="bubble" TEXT="messaging">
<font NAME="SansSerif" SIZE="18"/>
<node COLOR="#00b439" CREATED="1238592902441" ID="Freemind_Link_299437574" MODIFIED="1238595204252" TEXT="Core Interprocess">
<font NAME="SansSerif" SIZE="16"/>
<node COLOR="#990000" CREATED="1238593131801" ID="Freemind_Link_1869234103" MODIFIED="1238593154661" TEXT="Use Google Protocal Buffers">
<font NAME="SansSerif" SIZE="14"/>
<node COLOR="#111111" CREATED="1238580649707" ID="Freemind_Link_237713623" MODIFIED="1238594675608" TEXT="Each message must be prefixed by length as uint32 NBO ">
<node COLOR="#111111" CREATED="1238594217637" ID="Freemind_Link_186500699" MODIFIED="1238594232262" TEXT="Java requires no conversion"/>
<node COLOR="#111111" CREATED="1238594233184" ID="Freemind_Link_1221646658" MODIFIED="1238594277199" TEXT="C++ uses ntohl() and htonl() for conversion"/>
<node COLOR="#111111" CREATED="1238594280074" ID="Freemind_Link_1622837832" MODIFIED="1238594304418" TEXT="Python uses ntohl() and htonl() for conversion"/>
</node>
<node COLOR="#111111" CREATED="1238586910527" ID="Freemind_Link_893584045" MODIFIED="1238588126488" TEXT="Individual message should not exceed 1mb in total size."/>
<node COLOR="#111111" CREATED="1238586994387" ID="Freemind_Link_1927683379" MODIFIED="1238595677505" TEXT="All messages in a single channel must be contained in a single container messager type"/>
</node>
<node COLOR="#990000" CREATED="1238580036096" ID="Freemind_Link_1356505638" MODIFIED="1238633057285" TEXT="tcp/ip streams">
<font NAME="SansSerif" SIZE="14"/>
<node COLOR="#111111" CREATED="1238580178028" ID="Freemind_Link_962224871" MODIFIED="1238592927925" TEXT="Monitoring">
<font NAME="SansSerif" SIZE="12"/>
<node COLOR="#111111" CREATED="1238580056752" ID="Freemind_Link_1656782611" MODIFIED="1238587063310" TEXT="ack/nak">
<node COLOR="#111111" CREATED="1238580067431" ID="Freemind_Link_1793952716" MODIFIED="1238595061626" TEXT="requires unique message ID per channel"/>
<node COLOR="#111111" CREATED="1238580101729" ID="Freemind_Link_818607888" MODIFIED="1238634850527" TEXT="all messages must be acked or naked"/>
</node>
<node COLOR="#111111" CREATED="1238580433122" ID="Freemind_Link_1452016515" MODIFIED="1238587063310" TEXT="message length limits"/>
<node COLOR="#111111" CREATED="1238632797154" ID="Freemind_Link_904275977" MODIFIED="1238632846329" TEXT="invalid message format"/>
<node COLOR="#111111" CREATED="1238583140873" ID="Freemind_Link_1360584703" MODIFIED="1238632856850" TEXT="invalid data in message"/>
<node COLOR="#111111" CREATED="1238580468102" ID="Freemind_Link_883699061" MODIFIED="1238632867079" TEXT="stream overflow/overrun"/>
<node COLOR="#111111" CREATED="1238634200998" ID="Freemind_Link_498293977" MODIFIED="1238634205796" TEXT="stream closure"/>
<node COLOR="#111111" CREATED="1238580292105" ID="Freemind_Link_238934488" MODIFIED="1238632885702" TEXT="timeouts">
<node COLOR="#111111" CREATED="1238593860478" ID="Freemind_Link_867120506" MODIFIED="1238593906916" TEXT="Ack/Nak recept"/>
<node COLOR="#111111" CREATED="1238593876572" ID="Freemind_Link_125338643" MODIFIED="1238593888431" TEXT="Stream inactivity"/>
</node>
<node COLOR="#111111" CREATED="1238583176365" ID="Freemind_Link_275882191" MODIFIED="1238632893242" TEXT="retries"/>
</node>
<node COLOR="#111111" CREATED="1238580202313" ID="Freemind_Link_1913987350" MODIFIED="1238592927925" TEXT="Error Recovery">
<font NAME="SansSerif" SIZE="12"/>
<node COLOR="#111111" CREATED="1238580853847" ID="Freemind_Link_742326140" MODIFIED="1238633182440" TEXT="bad message length will force stream resync"/>
<node COLOR="#111111" CREATED="1238580905062" ID="Freemind_Link_627317217" MODIFIED="1238633139826" TEXT="invalid format (GBP error) will force stream resync"/>
<node COLOR="#111111" CREATED="1238588060722" ID="Freemind_Link_1388473889" MODIFIED="1238588716820" TEXT="Retrys exceeded will force stream reset"/>
<node COLOR="#111111" CREATED="1238583098769" ID="Freemind_Link_1309047494" MODIFIED="1238587063325" TEXT="Invalid data in message forces message resend"/>
<node COLOR="#111111" CREATED="1238594052167" ID="Freemind_Link_138388048" MODIFIED="1238594077042" TEXT="Ack/Nak timeout forces message resnd"/>
<node COLOR="#111111" CREATED="1238594078698" ID="Freemind_Link_455421750" MODIFIED="1238594094901" TEXT="Stream inactivity timeout closes stream."/>
</node>
<node COLOR="#111111" CREATED="1238580276791" ID="Freemind_Link_1341284251" MODIFIED="1238592927925" TEXT="Initialization">
<font NAME="SansSerif" SIZE="12"/>
<node COLOR="#111111" CREATED="1238612313882" ID="Freemind_Link_1357530819" MODIFIED="1238613074025" TEXT="Ports selected from config file"/>
<node COLOR="#111111" CREATED="1238612409432" ID="Freemind_Link_1368575642" MODIFIED="1238612429609" TEXT="Host Search Order">
<node COLOR="#111111" CREATED="1238612959138" ID="Freemind_Link_120275326" MODIFIED="1238612999667" TEXT="Configuration file"/>
<node COLOR="#111111" CREATED="1238613001181" ID="Freemind_Link_508195630" MODIFIED="1238613019382" TEXT="localhost.localdomain"/>
</node>
<node COLOR="#111111" CREATED="1238612662248" ID="Freemind_Link_1674177083" MODIFIED="1238612798910" TEXT="Exceptional Cases">
<node COLOR="#111111" CREATED="1238612814074" ID="Freemind_Link_1789939424" MODIFIED="1238612855307" TEXT="Connection Refused"/>
<node COLOR="#111111" CREATED="1238612856625" ID="Freemind_Link_1170804619" MODIFIED="1238612865601" TEXT="Host not found"/>
<node COLOR="#111111" CREATED="1238612867023" ID="Freemind_Link_1619793302" MODIFIED="1238612897476" TEXT="No route to host"/>
<node COLOR="#111111" CREATED="1238612898472" ID="Freemind_Link_993720256" MODIFIED="1238612955915" TEXT="All other cases"/>
</node>
<node COLOR="#111111" CREATED="1238633947835" ID="Freemind_Link_1257865311" MODIFIED="1238633970271" TEXT="No stream header"/>
</node>
</node>
<node COLOR="#990000" CREATED="1238632448782" ID="Freemind_Link_1112385195" MODIFIED="1238633336311" TEXT="file streams">
<font NAME="SansSerif" SIZE="14"/>
<node COLOR="#111111" CREATED="1238632519285" ID="Freemind_Link_370075918" MODIFIED="1238633425693" TEXT="Monitoring">
<font NAME="SansSerif" SIZE="12"/>
<node COLOR="#111111" CREATED="1238632647865" ID="Freemind_Link_883443697" MODIFIED="1238632694575" TEXT="message length limits"/>
<node COLOR="#111111" CREATED="1238632727840" ID="Freemind_Link_289978969" MODIFIED="1238632786710" TEXT="invalid message format"/>
<node COLOR="#111111" CREATED="1238632910951" ID="Freemind_Link_183275352" MODIFIED="1238632922268" TEXT="invalid data in message"/>
<node COLOR="#111111" CREATED="1238634178476" ID="Freemind_Link_1611351438" MODIFIED="1238634186640" TEXT="end of file"/>
</node>
<node COLOR="#111111" CREATED="1238633430828" ID="Freemind_Link_1408263157" MODIFIED="1238633438393" TEXT="Error Recovery">
<node COLOR="#111111" CREATED="1238580853847" ID="Freemind_Link_1741586176" MODIFIED="1238633182440" TEXT="bad message length will force stream resync"/>
<node COLOR="#111111" CREATED="1238580905062" ID="Freemind_Link_1397434225" MODIFIED="1238633139826" TEXT="invalid format (GBP error) will force stream resync"/>
<node COLOR="#111111" CREATED="1238633510505" ID="Freemind_Link_737702256" MODIFIED="1238633548697" TEXT="invalid data in message throws and discards message"/>
<node COLOR="#111111" CREATED="1238633569003" ID="Freemind_Link_189929359" MODIFIED="1238634908950" TEXT="EOF throws and discards current action"/>
</node>
<node COLOR="#111111" CREATED="1238633444734" ID="Freemind_Link_1919675425" MODIFIED="1238633452330" TEXT="Initialization">
<node COLOR="#111111" CREATED="1238634036544" ID="Freemind_Link_600600299" MODIFIED="1238634136344" TEXT="exceptional cases">
<node COLOR="#111111" CREATED="1238634139573" ID="Freemind_Link_1762828388" MODIFIED="1238634149102" TEXT="insufficent permissions"/>
<node COLOR="#111111" CREATED="1238634151248" ID="Freemind_Link_255048131" MODIFIED="1238634157462" TEXT="out of space"/>
<node COLOR="#111111" CREATED="1238634158720" ID="Freemind_Link_1311655284" MODIFIED="1238634163242" TEXT="path not found"/>
</node>
<node COLOR="#111111" CREATED="1238634010146" ID="Freemind_Link_1139252589" MODIFIED="1238634119725" TEXT="no stream header"/>
</node>
</node>
<node COLOR="#990000" CREATED="1238580240262" ID="Freemind_Link_1097604891" MODIFIED="1238592931128" TEXT="messages">
<font NAME="SansSerif" SIZE="14"/>
<node COLOR="#111111" CREATED="1238592682502" ID="Freemind_Link_1562410471" MODIFIED="1238592931128" TEXT="Common">
<font NAME="SansSerif" SIZE="12"/>
<node COLOR="#111111" CREATED="1238592719361" ID="Freemind_Link_1674539071" MODIFIED="1238592723658" TEXT="ack"/>
<node COLOR="#111111" CREATED="1238592730221" ID="Freemind_Link_999017803" MODIFIED="1238592735627" TEXT="nak"/>
<node COLOR="#111111" CREATED="1238592956675" ID="Freemind_Link_1546611878" MODIFIED="1238592971191" TEXT="Comon Physics Elements"/>
</node>
<node COLOR="#111111" CREATED="1238592695627" ID="Freemind_Link_1118049012" MODIFIED="1238595677505" TEXT="Channel Containers">
<font NAME="SansSerif" SIZE="12"/>
<node COLOR="#111111" CREATED="1238592782831" ID="Freemind_Link_1036657635" MODIFIED="1238592808628" TEXT="Sim Engine to Data Broker"/>
<node COLOR="#111111" CREATED="1238592814362" ID="Freemind_Link_671173031" MODIFIED="1238592824753" TEXT="Data Broker to Sim Engine"/>
<node COLOR="#111111" CREATED="1238592825409" ID="Freemind_Link_1287058589" MODIFIED="1238592855409" TEXT="Data Broker to Visualization"/>
<node COLOR="#111111" CREATED="1238592856081" ID="Freemind_Link_533888988" MODIFIED="1238592864800" TEXT="Visualization to Data Broker"/>
</node>
</node>
</node>
<node COLOR="#00b439" CREATED="1238595216034" ID="Freemind_Link_799643158" MODIFIED="1238595231737" TEXT="Bot Control Program">
<font NAME="SansSerif" SIZE="16"/>
</node>
</node>
<node COLOR="#0033ff" CREATED="1238583013010" ID="Freemind_Link_108958501" MODIFIED="1238588156848" POSITION="left" STYLE="bubble" TEXT="Simulation Engine">
<font NAME="SansSerif" SIZE="18"/>
<node COLOR="#00b439" CREATED="1238587749142" ID="Freemind_Link_217085795" MODIFIED="1238587811877" TEXT="Simulation Management">
<font NAME="SansSerif" SIZE="16"/>
<node COLOR="#990000" CREATED="1238587732861" ID="Freemind_Link_258191186" MODIFIED="1238592465438" TEXT="Physics Core">
<font NAME="SansSerif" SIZE="14"/>
</node>
<node COLOR="#990000" CREATED="1238587777486" ID="Freemind_Link_1777512296" MODIFIED="1238592426672" TEXT="Bot Management">
<font NAME="SansSerif" SIZE="14"/>
<node COLOR="#111111" CREATED="1238588330974" ID="Freemind_Link_893069488" MODIFIED="1238592426672" TEXT="Plugins">
<font NAME="SansSerif" SIZE="12"/>
</node>
</node>
<node COLOR="#990000" CREATED="1238587766830" ID="Freemind_Link_1648497015" MODIFIED="1238592441219" TEXT="Arena Management">
<font NAME="SansSerif" SIZE="14"/>
</node>
</node>
<node COLOR="#00b439" CREATED="1238587814736" ID="Freemind_Link_1701059403" MODIFIED="1238587844846" TEXT="Data Broker Communications">
<font NAME="SansSerif" SIZE="16"/>
</node>
</node>
<node COLOR="#0033ff" CREATED="1238583027818" ID="Freemind_Link_715915479" MODIFIED="1238587063341" POSITION="left" TEXT="Data Broker">
<font NAME="SansSerif" SIZE="18"/>
<node COLOR="#00b439" CREATED="1238587667642" ID="Freemind_Link_1911142768" MODIFIED="1238588172520" TEXT="Database">
<font NAME="SansSerif" SIZE="16"/>
</node>
<node COLOR="#00b439" CREATED="1238588696632" ID="Freemind_Link_890533326" MODIFIED="1238588716836" TEXT="Communciations">
<font NAME="SansSerif" SIZE="16"/>
<node COLOR="#990000" CREATED="1238588723633" ID="Freemind_Link_801776806" MODIFIED="1238588730742" TEXT="Simulation Engine">
<font NAME="SansSerif" SIZE="14"/>
</node>
<node COLOR="#990000" CREATED="1238588734320" ID="Freemind_Link_299148351" MODIFIED="1238588746211" TEXT="Visualization Clients">
<font NAME="SansSerif" SIZE="14"/>
</node>
</node>
</node>
<node COLOR="#0033ff" CREATED="1238583042095" ID="Freemind_Link_1842810078" MODIFIED="1238587063341" POSITION="left" TEXT="Visualization">
<font NAME="SansSerif" SIZE="18"/>
</node>
</node>
</map>
