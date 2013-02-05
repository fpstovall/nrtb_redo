<map version="0.9.0">
<!-- To view this file, download free mind mapping software FreeMind from http://freemind.sourceforge.net -->
<node CREATED="1199190455281" ID="ID_1574918623" MODIFIED="1199190480522" TEXT="thread_pool">
<node CREATED="1360081563697" ID="ID_729586653" MODIFIED="1360082647769" POSITION="right" TEXT="global pool manager()">
<node CREATED="1360082650811" ID="ID_1732889072" MODIFIED="1360082831445" TEXT="Free running thread"/>
<node CREATED="1360082832267" ID="ID_630625220" MODIFIED="1360082838829" TEXT="On startup">
<node CREATED="1360082840526" ID="ID_715527423" MODIFIED="1360082857649" TEXT="places thisTid in an accessable location"/>
<node CREATED="1360082860198" ID="ID_1561203760" MODIFIED="1360082884250" TEXT="prepares a clear task pool list"/>
</node>
<node CREATED="1360082900551" ID="ID_317519965" MODIFIED="1360082911045" TEXT="service loop">
<node CREATED="1360082912723" ID="ID_1377548506" MODIFIED="1360082954302" TEXT="rcv Tid, string">
<node CREATED="1360083028525" ID="ID_1807781309" MODIFIED="1360083218933" TEXT="return task pool Tid if exists"/>
<node CREATED="1360083086752" ID="ID_1950163396" MODIFIED="1360083109710" TEXT="if string not registered, return excepiton"/>
</node>
<node CREATED="1360082955756" ID="ID_744173745" MODIFIED="1360083256922" TEXT="rcv Tid, Tid id, bool">
<node CREATED="1360083260726" ID="ID_1876022998" MODIFIED="1360083296103" TEXT="if id exists and bool is false, deregister id"/>
<node CREATED="1360083310005" ID="ID_1649250749" MODIFIED="1360083323289" TEXT="if id does not exist, return exception"/>
</node>
<node CREATED="1360083007935" ID="ID_966938546" MODIFIED="1360083025868" TEXT="rcv Tid, Tid, string">
<node CREATED="1360083342249" ID="ID_730280620" MODIFIED="1360083450324" TEXT="if name|Tid not in use, register name/Tid pairing"/>
<node CREATED="1360083398978" ID="ID_38980067" MODIFIED="1360083434976" TEXT="If name|Tid are used, return excepiton"/>
</node>
</node>
</node>
<node CREATED="1360082136187" ID="ID_181135344" MODIFIED="1360082187227" POSITION="right" TEXT="task pool manager(t)(string name)"/>
<node CREATED="1360081583633" ID="ID_1103044446" MODIFIED="1360082218064" POSITION="right" TEXT="worker thread(t)(Tid task_pool_manager)"/>
<node CREATED="1360081590643" ID="ID_1324728" MODIFIED="1360081668955" POSITION="right" TEXT="struct work_packet(t)"/>
<node CREATED="1360081723340" ID="ID_1492950840" MODIFIED="1360081762970" POSITION="right" TEXT="struct limits"/>
<node CREATED="1360081739372" ID="ID_452972575" MODIFIED="1360081747882" POSITION="right" TEXT="struct run_ctl"/>
</node>
</map>
