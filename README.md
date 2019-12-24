# EgoNetConstructorAndSLPA
Example C++ implementation of constructing ego-nets from large graphs and SLPA community detection algorithm with performance optimization.

The ego-net construction algorithm implemented is from the paper *Epasto A, Lattanzi S, Mirrokni V, et al. Ego-net community mining applied to
friend suggestion[J]. Proceedings of the VLDB Endowment, 2015, 9(4): 324-335.*

The SLPA algorithm is from *Xie J, Szymanski B K, Liu X. Slpa: Uncovering overlapping communities in social
networks via a speaker-listener interaction dynamic process[C]//2011 ieee 11th
international conference on data mining workshops. IEEE, 2011: 344-349.*

Some language features are used to avoid any extra time and memory complexity, such as rvalue reference, object move (C++11), string_view (C++17), 
from_chars (C++17), OpenMP and so on.
