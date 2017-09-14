% Input Samples n*3 matrix and compute gaussian MLE parameters. No GMM as of yet.
% Author: Dhruv Ilesh Shah

Samples = double(Samples);
mu = sum(Samples, 1) / size(Samples, 1);

MU = repmat(mu, size(Samples, 1));
MU = MU(:, 1:numel(mu));

S = Samples - MU;
sigma = (S'*S) / size(Samples, 1);