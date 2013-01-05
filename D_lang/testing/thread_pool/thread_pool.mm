<map version="0.9.0">
<!-- To view this file, download free mind mapping software FreeMind from http://freemind.sourceforge.net -->
<node CREATED="1199190455281" ID="ID_1574918623" MODIFIED="1199190480522" TEXT="thread_pool">
<node CREATED="1357355872013" ID="ID_1802947813" MODIFIED="1357357270302" POSITION="right" TEXT="@property uint thread_count"/>
<node CREATED="1357356029266" ID="ID_138993489" MODIFIED="1357356345466" POSITION="right" TEXT="@property uint free_threads"/>
<node CREATED="1357358439387" ID="ID_1200218808" MODIFIED="1357358446392" POSITION="right" TEXT="@property low_water"/>
<node CREATED="1357358447507" ID="ID_1780124154" MODIFIED="1357358455528" POSITION="right" TEXT="@property high_water"/>
<node CREATED="1357358456371" ID="ID_1168370966" MODIFIED="1357358485632" POSITION="right" TEXT="@property increment"/>
<node CREATED="1357358491818" ID="ID_1188843464" MODIFIED="1357358571535" POSITION="right" TEXT="void set_levels(in uint low, in uint high, in uint increment)"/>
<node CREATED="1357358393243" ID="ID_742632506" MODIFIED="1357358401512" POSITION="right" TEXT="tid listener_tid"/>
<node CREATED="1357356274196" ID="ID_1073211966" MODIFIED="1357357779373" POSITION="right" TEXT="(T) // Data packet type"/>
<node CREATED="1357357780374" ID="ID_1278207219" MODIFIED="1357358422011" POSITION="right" TEXT="stuct {T data; tid requestor; bool done} work_packet"/>
<node CREATED="1357357090890" ID="ID_1871103626" MODIFIED="1357358436384" POSITION="right" TEXT="tid[] available_threads"/>
<node CREATED="1357356540428" ID="ID_948147143" MODIFIED="1357356844977" POSITION="right" TEXT="void enqueue(T) (in T data, in tid returnto)">
<node CREATED="1357358729705" ID="ID_71779585" MODIFIED="1357358773182" TEXT="if free_threads &lt; low_water start (increment) new threads"/>
<node CREATED="1357358260668" ID="ID_1129836507" MODIFIED="1357358320051" TEXT="assemble work_packet"/>
<node CREATED="1357358334211" ID="ID_116198003" MODIFIED="1357358343689" TEXT="set workpacket.done false"/>
<node CREATED="1357358344891" ID="ID_979460417" MODIFIED="1357412502419" TEXT="send work_packet to first available worker"/>
<node CREATED="1357412504102" ID="ID_1155499415" MODIFIED="1357412516125" TEXT="remove worker from available queue"/>
</node>
<node CREATED="1357356882411" ID="ID_1556184124" MODIFIED="1357356933049" POSITION="right" TEXT="void listener()">
<node CREATED="1357357018682" ID="ID_1539153576" MODIFIED="1357419298508" TEXT="on recept of (T)">
<node CREATED="1357413007865" ID="ID_1301738333" MODIFIED="1357413061232" TEXT="call enqueue(T, sender_tid)"/>
</node>
<node CREATED="1357357290001" ID="ID_662876327" MODIFIED="1357357438590" TEXT="on recept of this.close, shutdown all threads"/>
<node CREATED="1357357498008" ID="ID_819573878" MODIFIED="1357357542590" TEXT="on recept of this.available, list sender in available threads"/>
<node CREATED="1357357929342" ID="ID_630466535" MODIFIED="1357357958003" TEXT="on recept of work_packet">
<node CREATED="1357357958573" ID="ID_67627102" MODIFIED="1357357973435" TEXT="return data to requestor"/>
<node CREATED="1357357974501" ID="ID_265677790" MODIFIED="1357357986274" TEXT="list sender in available threads"/>
</node>
</node>
<node CREATED="1357358147828" ID="ID_1186684411" MODIFIED="1357358159690" POSITION="right" TEXT="void worker()">
<node CREATED="1357358162548" ID="ID_1797233738" MODIFIED="1357358180689" TEXT="on recept of work_packet, do the deed"/>
<node CREATED="1357358805513" ID="ID_1740349485" MODIFIED="1357358832325" TEXT="on startup, send this.available to listener_tid"/>
<node CREATED="1357358181756" ID="ID_12893510" MODIFIED="1357358939837" TEXT="when done, set work_packet.done = true and send to listener thread"/>
<node CREATED="1357358614842" ID="ID_1012337941" MODIFIED="1357358636510" TEXT="on recept of this.close, terminate the thread"/>
</node>
</node>
</map>
