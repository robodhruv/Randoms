% Load the required data set
load('student_data.mat');
% X = temp';
% Extract the first 3 rows of interest
Xn = X(1:3,:);

% Visualise the raw data
scatter3(Xn(1,:)',Xn(2,:)',0*Xn(3,:)', 'MarkerFaceColor',[0.9 0.9 0.9],...
    'MarkerEdgeColor','k');
hold on;
scatter3(Xn(1,:)',Xn(2,:)',Xn(3,:)', 50 ,Xn(3,:)', 'filled',...
    'MarkerEdgeColor','k'); colormap('hot');
for i = 1:size(Xn, 2)
    plot3([Xn(1,i) Xn(1,i)],[Xn(2,i) Xn(2,i)], [0 Xn(3,i)], '--',...
        'Color', '[0.5 0.5 0.5]');
end

% Calculate how many observations there are
N = size(Xn, 2);
% Compute the average value of each variable
mu = sum(Xn,2)./N

% Replicate the average vector into a matrix
mu_mat = repmat(mu,1,N);

% Subtract the average away from every column
Xhat = Xn - mu_mat;

% Preallocate the scaled matrix
Xhats = zeros(size(Xhat));

% Populate the scaled matrix by row
for i=1:3
    Xhats(i,:) = Xhat(i,:) / norm(Xhat(i,:)); 
end

% Compute the correlation matrix
C = Xhats * Xhats';

% Compute the sample covariance matrix
S = (1 / (N - 1)) * (Xhat * Xhat');

% Check that S is symmetric, the result should be a zero matrix
S' - S

% Compute the trace of S, i.e. the total variance
trace(S)

% Find the eigenvalues and eigenvectors of S
[Q,Dvec] = eig(S, 'vector')

% Sort in descending order
[Dvec, perm] = sort(Dvec, 'descend');
Q = Q(:, perm);
D = diag(Dvec);

% Show that Q is orthogonal
Q'*Q
Q*Q'

% Show that Q^T S Q is a diagonal matrix
Q'*S*Q

% Check the rank of Xhat transpose
rank(Xhat')

% Calculate the percentage of the variance accounted for by each principle
% component
D(1,1)/trace(S)
D(2,2)/trace(S)
D(3,3)/trace(S)

% Plot the original data with principal component axes and plane
t = [-50 50];
figure;
scatter3(Xn(1,:)',Xn(2,:)',Xn(3,:)', 'filled')
hold on
pc1_line = [sum(Xn,2)./N + t(1)*Q(:, 1), sum(Xn,2)./N + t(end)*Q(:, 1)];
plot3([pc1_line(1, 1) pc1_line(1, 2)], [pc1_line(2, 1) pc1_line(2, 2)],...
    [pc1_line(3, 1) pc1_line(3, 2)], '-g', 'LineWidth', 2);
pc2_line = [sum(Xn,2)./N + t(1)*Q(:, 2), sum(Xn,2)./N + t(end)*Q(:, 2)];
plot3([pc2_line(1, 1) pc2_line(1, 2)], [pc2_line(2, 1) pc2_line(2, 2)],...
    [pc2_line(3, 1) pc2_line(3, 2)], '-r', 'LineWidth', 2);

% Generate the plane with tangent vectors as first two principal components
Mu = sum(Xn,2)./N;
funx = @(s, t) Mu(1,1) + t*Q(1, 1) + s * Q(1, 2);
funy = @(s, t) Mu(2,1) + t*Q(2, 1) + s * Q(2, 2);
funz = @(s, t) Mu(3,1) + t*Q(3, 1) + s * Q(3, 2);
s = ezsurf(funx, funy, funz, t);
colormap('gray');
alpha(s, '0.5');
axis([0 30 0 10 0 60]);
