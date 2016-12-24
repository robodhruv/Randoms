# Class courtesy: Morvan Zhou
# Youtube video tutorial: https://www.youtube.com/channel/UCdyjiB5H8Pu7aDTNVXTTpcg

"""
Please note, this code is only for python 3+. If you are using python 2+, please modify the code accordingly.

Run this script on tensorflow r0.10. Errors appear when using lower versions.
"""
import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
import csv


BATCH_START = 0
TIME_STEPS = 20
BATCH_SIZE = 30
INPUT_SIZE = 1
OUTPUT_SIZE = 1
CELL_SIZE = 10
LR = 0.006


def load_csv(filename):
    dataset = list()
    with open(filename, 'r') as file:
        csv_reader = csv.reader(file)
        for row in csv_reader:
            if not row:
                continue
            dataset.append(row)
    return dataset

data = np.array(load_csv('data.csv'))
print "Done Reading!"
print (len(data))


def get_batch():
    global BATCH_START, TIME_STEPS, data
    # # data = np.array(load_csv('data.csv'))
    # xs_complete = data[:,0]
    # seq_complete
    xs = data[BATCH_START: BATCH_START+(TIME_STEPS*BATCH_SIZE), 0].reshape((BATCH_SIZE, TIME_STEPS))
    seq = data[BATCH_START: BATCH_START+(TIME_STEPS*BATCH_SIZE), 1].reshape((BATCH_SIZE, TIME_STEPS))
    res = data[BATCH_START+4: BATCH_START+(TIME_STEPS*BATCH_SIZE) +4, 1].reshape((BATCH_SIZE, TIME_STEPS))
    # xs shape (50batch, 20steps)
    # xs = np.arange(BATCH_START, BATCH_START+TIME_STEPS*BATCH_SIZE).reshape((BATCH_SIZE, TIME_STEPS)) / (10*np.pi)
    # xs_shifted = np.arange(BATCH_START+1, (BATCH_START+TIME_STEPS*BATCH_SIZE) +1).reshape((BATCH_SIZE, TIME_STEPS)) / (10*np.pi)
    # seq = 10*np.sin(xs)
    # res = 2*np.sin(xs_shifted)

    BATCH_START += TIME_STEPS
    # plt.plot(xs[0, :], res[0, :], 'r', xs[0, :], seq[0, :], 'b--')
    # plt.show()
    # returned seq, res and xs: shape (batch, step, input)
    return [seq[:, :, np.newaxis], res[:, :, np.newaxis], xs]


class LSTMRNN(object):
    def __init__(self, n_steps, input_size, output_size, cell_size, batch_size):
        self.n_steps = n_steps
        self.input_size = input_size
        self.output_size = output_size
        self.cell_size = cell_size
        self.batch_size = batch_size
        with tf.name_scope('inputs'):
            self.xs = tf.placeholder(tf.float32, [None, n_steps, input_size], name='xs')
            self.ys = tf.placeholder(tf.float32, [None, n_steps, output_size], name='ys')
        with tf.variable_scope('in_hidden'):
            self.add_input_layer()
        with tf.variable_scope('LSTM_cell'):
            self.add_cell()
        with tf.variable_scope('out_hidden'):
            self.add_output_layer()
        with tf.name_scope('cost'):
            self.compute_cost()
        with tf.name_scope('train'):
            self.train_op = tf.train.AdamOptimizer(LR).minimize(self.cost)

    def add_input_layer(self,):
        l_in_x = tf.reshape(self.xs, [-1, self.input_size], name='2_2D')  # (batch*n_step, in_size)
        # Ws (in_size, cell_size)
        Ws_in = self._weight_variable([self.input_size, self.cell_size])
        # bs (cell_size, )
        bs_in = self._bias_variable([self.cell_size,])
        # l_in_y = (batch * n_steps, cell_size)
        with tf.name_scope('Wx_plus_b'):
            l_in_y = tf.matmul(l_in_x, Ws_in) + bs_in
        # reshape l_in_y ==> (batch, n_steps, cell_size)
        self.l_in_y = tf.reshape(l_in_y, [-1, self.n_steps, self.cell_size], name='2_3D')

    def add_cell(self):
        lstm_cell = tf.nn.rnn_cell.BasicLSTMCell(self.cell_size, forget_bias=1.0, state_is_tuple=True)
        with tf.name_scope('initial_state'):
            self.cell_init_state = lstm_cell.zero_state(self.batch_size, dtype=tf.float32)
        self.cell_outputs, self.cell_final_state = tf.nn.dynamic_rnn(
            lstm_cell, self.l_in_y, initial_state=self.cell_init_state, time_major=False)

    def add_output_layer(self):
        # shape = (batch * steps, cell_size)
        l_out_x = tf.reshape(self.cell_outputs, [-1, self.cell_size], name='2_2D')
        Ws_out = self._weight_variable([self.cell_size, self.output_size])
        bs_out = self._bias_variable([self.output_size, ])
        # shape = (batch * steps, output_size)
        with tf.name_scope('Wx_plus_b'):
            self.pred = tf.matmul(l_out_x, Ws_out) + bs_out

    def compute_cost(self):
        losses = tf.nn.seq2seq.sequence_loss_by_example(
            [tf.reshape(self.pred, [-1], name='reshape_pred')],
            [tf.reshape(self.ys, [-1], name='reshape_target')],
            [tf.ones([self.batch_size * self.n_steps], dtype=tf.float32)],
            average_across_timesteps=True,
            softmax_loss_function=self.ms_error,
            name='losses'
        )
        with tf.name_scope('average_cost'):
            self.cost = tf.div(
                tf.reduce_sum(losses, name='losses_sum'),
                self.batch_size,
                name='average_cost')
            tf.scalar_summary('cost', self.cost)

    def ms_error(self, y_pre, y_target):
        return tf.square(tf.sub(y_pre, y_target))

    def _weight_variable(self, shape, name='weights'):
        initializer = tf.random_normal_initializer(mean=0., stddev=0.5,)
        return tf.get_variable(shape=shape, initializer=initializer, name=name)

    def _bias_variable(self, shape, name='biases'):
        initializer = tf.constant_initializer(0.1)
        return tf.get_variable(name=name, shape=shape, initializer=initializer)


if __name__ == '__main__':
    model = LSTMRNN(TIME_STEPS, INPUT_SIZE, OUTPUT_SIZE, CELL_SIZE, BATCH_SIZE)
    sess = tf.Session()
    merged = tf.merge_all_summaries()
    writer = tf.train.SummaryWriter("logs", sess.graph)
    # tf.initialize_all_variables() no long valid from
    # 2017-03-02 if using tensorflow >= 0.12
    sess.run(tf.global_variables_initializer())
    # sess.run(tf.initialize_all_variables())

    # relocate to the local dir and run this line to view it on Chrome (http://0.0.0.0:6006/):
    # $ tensorboard --logdir='logs'

    res_all = []
    pred_all = []
    seq_all = []
    # plt.ion()
    # plt.show()
    for i in range((len(data)-BATCH_SIZE*TIME_STEPS)/TIME_STEPS):
        seq, res, xs = get_batch()
        pred = []
        if i == 0:
            feed_dict = {
                    model.xs: seq,
                    model.ys: res,
                    # create initial state
            }
        else:
            feed_dict = {
                model.xs: seq,
                model.ys: res,
                model.cell_init_state: state    # use last state as the initial state for this run
            }

        _, cost, state, pred = sess.run(
            [model.train_op, model.cost, model.cell_final_state, model.pred],
            feed_dict=feed_dict)

        res_all.extend(res[0].flatten())
        seq_all.extend(seq[0].flatten())
        pred_all.extend(pred.flatten()[:TIME_STEPS])
        # store.append([res[0].flatten(), pred.flatten()[:TIME_STEPS]])
            # plotting
        # plt.plot(xs[0, :], res[0].flatten(), 'r', xs[0, :], pred.flatten()[:TIME_STEPS], 'b--')
        # plt.ylim((0.4, 0.6))
        # plt.draw()
        # plt.pause(0.00000000000001)

        if i % 20 == 0:
            print('cost: %s' %round(cost, 4))
            result = sess.run(merged, feed_dict)
            writer.add_summary(result, i)
    print "Writing Now."
    with open('output.csv', 'wb') as file:
        csvwriter = csv.writer(file)
        for i in range(len(res_all)-1):
            csvwriter.writerow([res_all[i], seq_all[i], pred_all[i]])

        seq = seq.flatten()
        res = res.flatten()
        pred = pred.flatten()
        for i in range(len(seq)):
            csvwriter.writerow([res[i], seq[i], pred[i]])

        # csvwriter.writerow(pred_all)
