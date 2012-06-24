class BST_Tree(object):
	def __init__(self, value):
		self.value = value
		self.child = [None, None]

	def insert(self, value):
		idx = 1 if (cmp(value, self.value) >= 0) else 0
		if self.child[idx]:
			self.child[idx].insert(value)
		else:
			self.child[idx] = type(self)(value)

	def search(self, value):
		c = cmp(value, self.value)
		if c == 0:
			return self.value
		else:
			idx = 1 if (c > 0) else 0
			if self.child[idx]:
				return self.child[idx].search(value)
			else:
				return None

	def __str__(self):
		if any(self.child):
			ns = self.node_str()
			(lstr, rstr) = [str(self.child[idx]).split('\n') if self.child[idx] else [' '] for idx in [0,1]]
			ret = ' '*len(lstr[0]) + ns + ' '*len(rstr[0])
			if len(lstr) > len(rstr):
				rstr.extend([' '*len(rstr[0])]*(len(lstr)-len(rstr)))
			elif len(rstr) > len(lstr):
				lstr.extend([' '*len(lstr[0])]*(len(rstr)-len(lstr)))
			ret = '\n'.join([ret] + [l+' '*len(ns)+r for (l,r) in zip(lstr, rstr)])
		else:
			ret = ' ' + self.node_str() + ' '

		return ret

	def node_str(self):
		return '%g(%d)' % (self.value, self.balance)

class AVL_Tree(object):
	def __init__(self, value=None):
		if value is None:
			self.root = None
		else:
			self.root = AVL_Node(value)

	def insert(self, value):
		if self.root is None:
			self.root = AVL_Node(value)
		else:
			(self.root, grew) = self.root.insert(value)

	def search(self, value):
		if self.root is None:
			return None
		else:
			return self.root.search(value)

	def delete(self, value):
		if self.root is None:
			raise ValueError('%r not in tree' % value)
		else:
			(self.root, shrank) = self.root.delete(value)

	def __str__(self):
		return str(self.root)

	def __iter__(self):
		if self.root:
			for n in self.root:
				yield n

class AVL_Node(BST_Tree):
	def __init__(self, value):
		super(AVL_Node, self).__init__(value)
		self.balance = 0

	def insert(self, value):
		c = cmp(value, self.value)
		idx = 1 if (c >= 0) else  0
		sgn = 1 if (c >= 0) else -1

		if self.child[idx]:
			(self.child[idx], child_grew) = self.child[idx].insert(value)
			self.balance += child_grew * sgn
			(root, shrank) = self.rebalance()
			#In the insert case, shrank is always true IF the tree was rebalanced
			#If the tree was not rebalanced (!shrank), and self.balance != 0
			#AND the child grew, then this tree also grew
			grew = bool(child_grew and not shrank and self.balance)
			return (root, grew)
		else:
			self.child[idx] = type(self)(value)
			self.balance += sgn
			return (self, bool(self.balance))

	def delete(self, value):
		#return value is (new_root, shrank)
		c = cmp(value, self.value)
		if c == 0:
			if self.child[0] is None:
				return (self.child[1], True)
			elif self.child[1] is None:
				return (self.child[0], True)
			else:
				(picked, new_left, child_shrank) = self.child[0].pick_rightmost()
				picked.child[1] = self.child[1]
				picked.child[0] = new_left
				picked.balance = self.balance + child_shrank
				root = picked
		else:
			idx = 1 if (c > 0) else  0
			sgn = 1 if (c > 0) else -1
			if self.child[idx]:
				(self.child[idx], child_shrank) = self.child[idx].delete(value)
				self.balance -= child_shrank * sgn
			else:
				raise ValueError('%r not in tree' % value)
			root = self

		if child_shrank:
			if root.balance == 0:
				return (root, True)
			else:
				(root, shrank) = root.rebalance()
				return (root, shrank)
		else:
			return (root, False)

	def pick_rightmost(self):
		#return value is (picked, new_root, shrank)
		idx = 1
		sgn = 1
		if self.child[idx]:
			(picked, self.child[idx], shrank) = self.child[idx].pick_rightmost()
			self.balance -= shrank * sgn
			if shrank:
				if self.balance == 0:
					return (picked, self, True)
				else:
					(root, shrank) = self.rebalance()
					return (picked, root, shrank)
			else:
				return (picked, self, False)
		else:
			return (self, self.child[idx-1], True)

	def rot(self, sgn):
		idx = 1 if (sgn == 1) else 0
		tmp = self.child[idx-1]
		self.child[idx-1] = tmp.child[idx]
		tmp.child[idx] = self
		return tmp

	def rrot(self):
		return self.rot(1)

	def lrot(self):
		return self.rot(-1)

	def rebalance(self):
		if abs(self.balance) < 2:
			root = self
			shrank = False
		else:
			sgn = 1 if self.balance == 2 else -1
			idx1 = 1 if self.balance == 2 else 0
			child1 = self.child[idx1]
			shrank = bool(child1.balance)
			if child1.balance == sgn:
				self.balance = 0
				child1.balance = 0
			elif child1.balance == 0:
				self.balance = sgn
				child1.balance = -sgn
			elif child1.balance == -sgn:
				idx2 = abs(idx1-1)
				child2 = child1.child[idx2]
				(self.balance, child1.balance) = \
						(-sgn,0) if (child2.balance ==  sgn) else \
						(0, sgn) if (child2.balance == -sgn) else \
						(0,   0) #if (child2.balance ==    0)
				child2.balance = 0
				self.child[idx1] = child1.rot(sgn)
			else:
				raise ValueError('invalid balance')
			root = self.rot(-sgn)

		return (root, shrank)

	def __iter__(self):
		if self.child[0]:
			for n in self.child[0]:
				yield n
		yield self.value
		if self.child[1]:
			for n in self.child[1]:
				yield n

if __name__ == '__main__':
	t = AVL_Tree()
	for v in [0,3,2,4,-2,-4]:
		t.insert(v)
	print t
	print [n for n in t]

	if True:
		N = 1000000
		import random
		ilist = []
		t = AVL_Tree()
		for n in xrange(N):
			try:
				r = random.randint(0, 100)
				ilist.append(r)
				t.insert(r)
			except BaseException as e:
				print t
				print ilist
				raise e

		tlist = [n for n in t]
		if tlist == sorted(tlist):
			cont = True
			print "Rand insert test passed!"
		else:
			cont = False
			print "Rand insert test failed!"
			#print t
			#print ilist

		if cont:
			rlist = []
			for n in xrange(N/2):
				try:
					r = random.randint(0, len(tlist)-1)
					rlist.append(tlist[r])
					t.delete(tlist[r])
					del tlist[r]
				except ValueError as e:
					print t
					print tlist
					raise e

			if tlist == [n for n in t]:
				print "Rand delete test passed!"
			else:
				print "Rand delete test failed!"
