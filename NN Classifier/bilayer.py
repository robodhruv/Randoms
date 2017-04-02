from numpy import exp, array, random, dot, sum, size, absolute


class NeuronLayer():
	# Declaring a layer of neurons, and their synaptic weights

	def __init__(self, number_of_neurons, number_of_inputs_per_neuron):
		self.synaptic_weights = 2 * \
			random.random((number_of_inputs_per_neuron, number_of_neurons)) - 1


class NeuralNetwork():

	def __init__(self, layer1, layer2):

		self.layer1 = layer1
		self.layer2 = layer2
		# The Sigmoid function, which describes an S shaped curve.
		# We pass the weighted sum of the inputs through this function to
		# normalise them between 0 and 1 and to capture higher order features
		# that any linear function would miss.

	def __sigmoid(self, x): 
		return 1 / (1 + exp(-x))

	# The derivative of the Sigmoid function.
	# This is the gradient of the Sigmoid curve.
	# It indicates how confident we are about the existing weight.
	def __sigmoid_derivative(self, x):
		y = self.__sigmoid(x)
		return y * (1 - y)

	# We train the neural network through a process of trial and error.
	# Adjusting the synaptic weights each time.
	def train(
			self,
			training_set_inputs,
			training_set_outputs, epsilon, max_iterations):
		cost = 1
		count = 1
		cost_prev = 0
		# for iteration in xrange(number_of_iterations):
		while abs(cost_prev - cost) > epsilon:
			cost_prev = cost
			# while absolute(cost - cost_prev) > epsilon:
			# Pass the training set through our neural network (a single
			# neuron).
			#cost_prev = cost
			output_1, output_2 = self.think(training_set_inputs)

			# Calculate the error (The difference between the desired output
			# and the predicted output after second layer).
			output_error = training_set_outputs - output_2
			output_delta = output_error * self.__sigmoid_derivative(output_2)
			# From theory, this is the amount of adjustment in the second
			# neuron layer by back-prop. Note that the update expression would
			# be towards the end because the backprop expression of layer 1
			# must be based on the older values.

			# Now we calculate the layer1 deltas which follow from the
			# cross-synaptic relations
			layer1_error = dot(output_delta, self.layer2.synaptic_weights.T)
			layer1_delta = layer1_error * self.__sigmoid_derivative(output_1)

			cost = sum(abs(output_error**2))

			# Now that we have the deltas, let's make the adjustments!
			# Showtime!
			layer1_adj = dot(training_set_inputs.T, layer1_delta)
			layer2_adj = dot(output_1.T, output_delta)

			# Updating the synaptic weights
			self.layer1.synaptic_weights += layer1_adj
			self.layer2.synaptic_weights += layer2_adj
			kilocount = count / 1000
			count += 1
			if count / 1000 > kilocount:
				print count / 1000, ":", abs(cost - cost_prev)
			if count > max_iterations:
				break
		print count

	# The neural network thinks.
	def think(self, inputs):
		# Pass inputs through our neural network:
		output_1 = self.__sigmoid(dot(inputs, self.layer1.synaptic_weights))
		output_2 = self.__sigmoid(dot(output_1, self.layer2.synaptic_weights))
		return output_1, output_2


if __name__ == "__main__":

	# Create the neuron layers as desired. Let's say we want a network with 3x4x1 neurons, layer-wise.
	# Thus the hidden layer would have 3 inputs for the 4 neurons each. The
	# weights matrix would thus be 4x3
	layer1 = NeuronLayer(4, 3)
	layer2 = NeuronLayer(1, 4)

	brain = NeuralNetwork(layer1, layer2)

	# Declare the training set. Here I have taken the case of an XOR gate with
	# the last two bits and the first is not considered.
	inputs = array([[0, 0, 1], [0, 1, 1], [1, 0, 1], [
				   0, 1, 0], [1, 0, 0], [1, 1, 1], [0, 0, 0]])
	outputs = array([[1, 0, 1, 1, 0, 0, 0]]).T

	# Time to train the neural network! An upper limit can be specified, at which it
	# should exit if no convergence is obtained. The value of cost convergence
	# desired (difference between the costs of two successive iterations) can
	# also be specified.
	epsilon = 1e-8
	max_iter = 50000

	brain.train(inputs, outputs, epsilon, max_iter)

	hidden_layer, output = brain.think(array([1, 1, 0]))
	print "Output for the unspecified case of [1, 1, 0]:", output
