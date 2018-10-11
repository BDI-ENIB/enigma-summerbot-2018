## Small Scale PathFinder Lib
Based on a dijkstra algorithm
# usage
~~~~
PathFinder pf;
PathFinderNode node1* new PathFinderNode(0,0);
PathFinderNode node2* new PathFinderNode(10,10);
node1->addLink(node2);
pf.addNode(node1);
pf.addNode(node2);
PathFinderNode* s1 = pf.findPath(node1,node2);
PathFinderNode* s2 = pf.findPath(0,1,node2);
PathFinderNode* s3 = pf.findPath(0,1,11,12);
~~~~
