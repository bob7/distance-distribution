# distance-distribution
Fast approximate distribution of distances for very large graphs/networks

Two simple,short but fast cpp scripts, reading graph from a file and iterating BFS randomly and delivering increasingly sharper  approximations of the distribution of distances (length of shortest paths). 

Was aimed at large real-world networks available from:

https://dango.rocks/datasets/

https://snap.stanford.edu/data/

https://thegraphsblog.wordpress.com/datasets/

amongst other places. Converges quite fast. The second script is multi-threaded using #include <pthread.h>
and is much faster for very large networks (in machines with many CPU cores).

Was used during the research published in:

The idemetric property: when most distances are (almost) the same
G.Barmpalias, N.Huang, A.Lewis-Pye, A.Li, X.Li, Y.Pan, T.Roughgarden
Proceedings of the Royal Society A vol 475(2222), 2019. 

https://royalsocietypublishing.org/doi/pdf/10.1098/rspa.2018.0283	
https://arxiv.org/abs/1804.11187

which explains the high concentration of the distribution on one or two values.

Output Example 1:

Dataset statistics
Nodes	23133
Edges	93497

reading size of graph CA-CondMat ......done: 108300 nodes and 186936 edges
initialization of arrays......done
reading the graph......done
calculating distances... (2000 BFSs in steps of 400)

	finished  400  BFSs in    6 seconds
	finished  800  BFSs in    6 seconds
	finished 1200  BFSs in    5 seconds
	finished 1600  BFSs in   12 seconds
	finished 2000  BFSs in    6 seconds

	  1     0.0004205    0.0003846    0.0003861    0.0004565    0.0004507  
	  2     0.0052059    0.0045338    0.0043056    0.0049573    0.0050165  
	  3     0.0495020    0.0437862    0.0414398    0.0442287    0.0446414  
	  4     0.2115350    0.1978294    0.1941519    0.1996201    0.2006987  
	  5     0.3358996    0.3424168    0.3446185    0.3447050    0.3485394  
	  6     0.2450211    0.2592838    0.2610596    0.2549803    0.2543932  
	  7     0.1112193    0.1132331    0.1147803    0.1104898    0.1073800  
	  8     0.0327630    0.0310530    0.0317487    0.0323340    0.0310654  
	  9     0.0070177    0.0062654    0.0063235    0.0068629    0.0065267  
	 10     0.0012059    0.0010474    0.0010263    0.0011710    0.0011055  
	 11     0.0001789    0.0001424    0.0001360    0.0001674    0.0001576  
	 12     0.0000278    0.0000223    0.0000222    0.0000248    0.0000230  
	 13     0.0000033    0.0000017    0.0000015    0.0000021    0.0000020  
	 14     0.0000000    0.0000000    0.0000000    0.0000000    0.0000000  

	finished all in 36 seconds

Output Example 2:

Dataset statistics
Nodes	4039
Edges	88234

	reading size of graph facebook_combined ......done: 4039 nodes and 88234 edges
	initialization of arrays......done
	reading the graph......done
	calculating distances... (300 BFSs in steps of 50)

	finished   50  BFSs in    1 seconds
	finished  100  BFSs in    1 seconds
	finished  150  BFSs in    1 seconds
	finished  200  BFSs in    1 seconds
	finished  250  BFSs in    2 seconds
	finished  300  BFSs in    1 seconds

	  1     0.0137580    0.0129549    0.0110388    0.0114652    0.0114982    0.0112653  
 	  2     0.1613994    0.1689895    0.1728806    0.1750969    0.1714007    0.1716942  
	  3     0.2825315    0.2684777    0.2763427    0.2555099    0.2467095    0.2447648  
	  4     0.3378075    0.3452123    0.3452601    0.3540082    0.3600614    0.3635294  
	  5     0.1199018    0.1438163    0.1444509    0.1525744    0.1567203    0.1568442  
	  6     0.0532117    0.0423433    0.0360002    0.0349821    0.0362767    0.0354279  
	  7     0.0305512    0.0177839    0.0137453    0.0160072    0.0168455    0.0159428  
	  8     0.0008389    0.0004220    0.0002812    0.0003561    0.0004876    0.0005314  
	  9     0.0000000    0.0000000    0.0000000    0.0000000    0.0000000    0.0000000  

	finished all in 9 seconds
