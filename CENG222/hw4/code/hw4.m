pA = 0.012;                               % probability of compatibility for A
pB = 0.790;                               % probability of compatibility for B

z_a_2 = 2.326; eps = 0.03;                % P=0.98, Eps=0.03
N = ceil((z_a_2/eps)**2 / 4);             % >= 1502.8544, so 1503

A_num_tri = zeros(N, 1); A_ratio = zeros(N, 1);% containers for sparse case, A
B_num_tri = zeros(N, 1); B_ratio = zeros(N, 1);% containers for dense  case, B

for i = 1 : N;                            % SIMULATIONS
  num_good = poissrnd(160);               % sample number of goods
  G = rand(num_good, num_good);           % generate random NxN matrix
  
  G = G - triu(G) + tril(G)';             % copy lower half to upper half
                                          % to make it symmetric

  A = G <= pA;                            % sparse graph for A, G(i, j) <= pA
  B = G <= pB;                            % dense  graph for B, G(i, j) <= pB
  
  A_num_tri(i) = trace(A^3) / 6;          % triangle count of A
  B_num_tri(i) = trace(B^3) / 6;          % triangle count of B
  
  gc3 = nchoosek(num_good, 3);            % number of triplets
  
  A_ratio(i) = A_num_tri(i) / gc3;        % ratio of triangles/triplets for A
  B_ratio(i) = B_num_tri(i) / gc3;        % ratio of triangles/triplets for B
end;

                              % Part A %
partA = mean(A_num_tri < 2)               % P(shipment <= 1) for A
                              % Part B %
partB = mean(B_ratio > 0.5)               % P(ratio > 0.5) for B
                              % Part C %
partC_X_A = mean(A_num_tri)               % estimation of expected X for A
partC_Y_A = mean(A_ratio)                 % estimation of expected Y for A
partC_X_B = mean(B_num_tri)               % estimation of expected X for B
partC_Y_B = mean(B_ratio)                 % estimation of expected Y for B
                              % Part D %
partD_stdX_A = std(A_num_tri)             % estimation of Std(X) for A
partD_stdY_A = std(A_ratio)               % estimation of Std(Y) for A
partD_stdX_B = std(B_num_tri)             % estimation of Std(X) for B
partD_stdY_B = std(B_ratio)               % estimation of Std(Y) for B
