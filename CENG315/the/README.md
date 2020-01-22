# Important Nodes

&nbsp;&nbsp;&nbsp;&nbsp; In this exam you need to find the order of importance of nodes in a given (non-negative) weighted undirected graph where ***n*** indicates the number of nodes whereas ***edgeList*** represents the edges given as adjacency matrix.

&nbsp;&nbsp;&nbsp;&nbsp; The importance score of nodes are defined as, the sum of ratio of shortest-path passing through the given node to shortest-path over all node pairs. To be precise, given a graph ***G = (V,E);***

<p align="center">
  <img width="375" height="80" src="https://github.com/cahity/METU-CENG/blob/master/CENG315/the/scores.png">
</p>

where ***v*** ∈ ***V***, ***σ(s,t)*** is the shortest path from ***s*** to ***t*** and ***σ(s,t|v)*** is the shortest path from ***s*** to ***t*** passing through ***v***. Also, if ***s=t***, ***σ(s,t)=1*** and if ***v=s*** or ***v=t***, ***σ(s,t|v)=0***.

&nbsp;&nbsp;&nbsp;&nbsp; Note that, the given graph may contain disconnected components so scores for the nodes in each component should be computed only with respect to other nodes in that component: you don't need to compute shortest path between nodes that reside in different components. In other terms, simply ignore the term in summation if any one of s, t or v is disconnected.

&nbsp;&nbsp;&nbsp;&nbsp; Your code should return the number of disconnected components and fill the given array scores with importance ***scores*** of nodes in the indices of their ids.

### Constraints
* Array sizes are less than ***10<sup>3</sup>*** (and > ***0***).
* Solution should work at most in ***O(n<sup>3</sup>)*** complexity.

### Example I/O
<img align="right" width="300" height="160" src="https://github.com/cahity/METU-CENG/blob/master/CENG315/the/graph1.png">

```
n = 5, edgelist = 
0 0 2 5 3
0 0 0 2 3
2 0 0 1 0
5 2 1 0 4
3 3 0 4 0
       return 1
```

---

<img align="right" width="300" height="160" src="https://github.com/cahity/METU-CENG/blob/master/CENG315/the/graph2.png">

```
n = 6, edgelist = 
0 0 0 3 0 2
0 0 3 0 2 0
0 3 0 0 0 0
3 0 0 0 0 4
0 2 0 0 0 0
2 0 0 4 0 0
       return 2
```
