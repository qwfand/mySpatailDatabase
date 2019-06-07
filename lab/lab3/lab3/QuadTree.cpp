#include <set>
#include "QuadTree.h"

namespace lab3 {

	/*
	 * QuadNode
	 */
	void QuadNode::split(size_t capacity)
	{
		for (int i = 0; i < 4; ++i) {
			delete[]nodes[i];
			nodes[i] = NULL;
		}
		//��ȡ��ǰ��Χ��
		double X = bbox.getMaxX(), Y = bbox.getMaxY(), x = bbox.getMinX(), y = bbox.getMinY();
		nodes[0] = new QuadNode(Envelope(x, (x + X) / 2, (y + Y) / 2, Y));
		nodes[1] = new QuadNode(Envelope((x + X) / 2, X, (y + Y) / 2, Y));
		nodes[2] = new QuadNode(Envelope((x + X) / 2, X, y, (y + Y) / 2));
		nodes[3] = new QuadNode(Envelope(x, (x + X) / 2, y, (y + Y) / 2));

		for (int j = 0; j < 4; j++) {
			vector<Feature>::iterator p = features.begin();
			while (p != features.end())//Ϊnode���Ԫ��
			{
				if (nodes[j]->bbox.intersect((*p).getEnvelope()) || nodes[j]->bbox.contain((*p).getEnvelope()))//
				{
					nodes[j]->add((*p));
					//p = features.erase(p, p + 1);
				}
				p++;
			}
		}
		/*
		 for (int j = 0; j < 4; j++) {
		 for (int i = 0; i < features.size(); i++)//Ϊnode���Ԫ��
		{
			//features.at(i).getEnvelope().print();
			//cout << endl; nodes[j]->bbox.print(); cout << endl;
			if (nodes[j]->bbox.intersect(features.at(i).getEnvelope()) || nodes[j]->bbox.contain(features.at(i).getEnvelope()))
			{
				nodes[j]->add(features.at(i));
			}
		}*/

		features.clear();
		for (int i = 0; i < 4; i++)
		{
			if (nodes[i] != NULL)
			{
				if (nodes[i]->getFeatureNum() > capacity)
				{
					nodes[i]->split(capacity);
				}
			}
		}
		// Task 5.2 construction
		// Write your code here
	}

	void QuadNode::countNode(int& interiorNum, int& leafNum)
	{
		if (isLeafNode()) {
			++leafNum;
		}
		else {
			++interiorNum;
			for (int i = 0; i < 4; ++i)
				nodes[i]->countNode(interiorNum, leafNum);
		}
	}

	int QuadNode::countHeight(int height)
	{
		++height;
		if (!isLeafNode()) {
			int cur = height;
			for (int i = 0; i < 4; ++i) {
				height = max(height, nodes[i]->countHeight(cur));
			}
		}
		return height;
	}

	void QuadNode::rangeQuery(Envelope& rect, vector<Feature>& features)
	{
		if (!bbox.intersect(rect))
			return;

		// Task 6.2 range query
		// Write your code here
		bool isLeaf = true;
		for (int i = 0; i < 4; i++)//�ж��ǲ���Ҷ�ڵ�
			if (nodes[i] != NULL)
			{
				isLeaf = false;
				break;
			}
		if (isLeaf)//��Ҷ�ڵ����������
		{
			vector<Feature>::iterator p = this->features.begin();
			while (p != this->features.end())
			{
				if (rect.intersect((*p).getEnvelope()))
					features.push_back(*p);
				p++;
			}
		}
		else//����Ҷ�ڵ㣬�������ӽڵ�
			for (int j = 0; j < 4; j++)
			{
				nodes[j]->rangeQuery(rect, features);
			}
	}

	QuadNode* QuadNode::pointInLeafNode(double x, double y)
	{
		// Task 7.2 NN query
		// Write your code here
		if (!bbox.contain(x, y))
			return NULL;
		QuadNode *qd = NULL;
		bool isLeaf = true;
		for (int i = 0; i < 4; i++)//�ж��ǲ���Ҷ�ڵ�
			if (nodes[i] != NULL)
			{
				isLeaf = false;
				break;
			}
		if (isLeaf)//��Ҷ�ڵ�������ж�
		{
			if (bbox.contain(x, y))
			{
				qd = this;
				//cout << this << "in f1" << endl;
			}
		}
		else//����Ҷ�ڵ㣬�������ӽڵ�
			for (int j = 0; j < 4; j++)
			{
				qd = nodes[j]->pointInLeafNode(x, y);// != NULL ? nodes[j]->pointInLeafNode(x, y) : qd;
				if (qd != NULL)//�ҵ���Ҫ����
					return qd;
			}
		if (qd)
			return qd;
		else
			return NULL;
	}

	void QuadNode::draw()
	{
		if (isLeafNode()) {
			bbox.draw();
		}
		else {
			for (int i = 0; i < 4; ++i)
				nodes[i]->draw();
		}
	}

	/*
	 * QuadTree
	 */
	bool QuadTree::constructQuadTree(vector<Feature>& features)
	{
		if (features.empty())
			return false;

		// Task 5.1 construction
		// Write your code here

		Envelope e = features.at(0).getEnvelope();
		int i;
		for (i = 1; i < features.size(); i++)
		{
			e = e.unionEnvelope(features.at(i).getEnvelope());
		}
		root = new QuadNode(e);
		//root = new QuadNode(Envelope(-74.1, -73.8, 40.6, 40.8).unionEnvelope(e));//root�ڵ��ʼ��
		root->add(features);

		if (features.size() <= this->capacity)//��������capacityʱ,����Ҫ�ָ�
		{
			//return true;

			Envelope e1 = features.at(0).getEnvelope();
			for (int i = 1; i < features.size(); i++)
			{
				Envelope e2 = features.at(i).getEnvelope();
				e1 = e1.unionEnvelope(e2);
			}
			bbox = e1;
		}
		else
		{
			root->split(capacity);
		}

		bbox = e;//Envelope(-74.1, -73.8, 40.6, 40.8).unionEnvelope(e);//Envelope(-74.1, -73.8, 40.6, 40.8); 
				 // ע����д�����Ҫ����Ϊfeatures�İ�Χ�У�����ڵ�İ�Χ��

		return true;
	}

	void QuadTree::countQuadNode(int& interiorNum, int& leafNum)
	{
		interiorNum = 0;
		leafNum = 0;
		if (root)
			root->countNode(interiorNum, leafNum);
	}

	void QuadTree::countHeight(int &height)
	{
		height = 0;
		if (root)
			height = root->countHeight(0);
	}

	void QuadTree::rangeQuery(Envelope& rect, vector<Feature>& features)
	{
		features.clear();

		// Task 6.1 range query
		// Write your code here

		// filter step (ѡ���ѯ�����뼸�ζ����Χ���ཻ�ļ��ζ���)
		root->rangeQuery(rect, features);
		/*
		if (bbox.intersect(rect) != NULL)
		{
			if (root->getChildNode(0) != NULL)
				for (int i = 0; i < 4; i++)
					root->getChildNode(i)->rangeQuery(rect, features);
		}*/

		// refine step (��ȷ�ж�ʱ����Ҫȥ�أ������ѯ����ͼ��ζ�����ظ�����)
		//ȥ��
		vector<Feature>::iterator p = features.begin(), q;
		while (p != features.end())
		{
			q = p + 1;
			while (q != features.end())
			{
				if ((*p).getGeom() == (*q).getGeom())
					q = features.erase(q);
				else q++;
			}
			p++;
		}
		//jisuan
		p = features.begin();
		while (p != features.end())
		{
			if (rect.contain((*p).getEnvelope()))
				p++;
			else p = features.erase(p);
		}
	}

	bool QuadTree::NNQuery(double x, double y, Feature& feature)
	{
		if (!root || !(root->getEnvelope().contain(x, y)))
			return false;

		// Task 7.1 NN query
		// Write your code here

		// filter step (ʹ��maxDistance2Envelope��������ò�ѯ�㵽���ζ����Χ�е���̵������룬Ȼ�������ѯ��ú�ѡ��)

		QuadNode *qd = root->pointInLeafNode(x, y);
		//cout << x << " " << y << endl << qd << " in f2" << endl;
		//(*qd).getEnvelope().print(); cout << endl;
		
		const Envelope& envelope = qd->getEnvelope();
		double minDist = max(envelope.getWidth(), envelope.getHeight());

		Envelope E(x - minDist, x + minDist, y - minDist, y + minDist);

		vector<Feature> ff;
		if (qd != NULL)
		{
			//Envelope e = qd->getEnvelope();//Ĭ�ϱ������ڴ�������ģ�������Ҫ����Χ
			 this->rangeQuery(E, ff);
			 /*
			 while (ff.size() == 0)//Ĭ���ܲ�ѯ������ģ�������Ҫ����Χ
			 {
				 e = Envelope(e.getMinX() - 0.01, e.getMaxX() + 0.01, e.getMinY() - 0.01, e.getMaxY() + 0.01);
				 this->rangeQuery(e, ff);
			 }*/
			// cout << ff.size() << " get the features" << endl << endl;
		}

		// refine step (��ȷ�����ѯ���뼸�ζ���ľ���)
		Point point(x, y);
		vector<Feature> ::iterator p = ff.begin();
		Feature temp;
		while (p != ff.end())
		{
			if ((*p).distance(x, y) < minDist) {
				minDist = (*p).distance(x, y);
				temp = *p;
			}
			p++;
		}
		feature = temp;
		ff.clear();
		return true;
	}

	void QuadTree::draw()
	{
		if (root)
			root->draw();
	}

}
