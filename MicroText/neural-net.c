// Remo //
//Testing// IF YOU EDIT THE CODE PUT YOUR NAME NEXT TO IT 
//#include <vector>
#include <stdlib.h>

using namespace std;
struct Neuron;
typedef vector<Neuron> Layer;

struct Net
{
public:
	Net(const vector<unsigned> &topology);
	void feedForward(const vector<double> &inputVals);
	void backProp(const vector<double> &targetVals);
	void getResults(vector<double> &resulVals ) const;

private:
	vector<Layer> m_layers; //m_layers[layerNum][neuronNum]	
};

Net::Net(const vector<unsigned> &topology)
{
	unsigned numLayer = topology.size();
	for(unsigned layerNum = 0; layerNum < numLayers; ++layerNum)
	{
		m_layers.push_back(Layer());
		for (unsigned neuronNUm =0; neuronNum <= topology <= topology[layerNum];++neuronNum)
		{
			m_layers.back().push_back(Neuraon());	
		}
	}
}


int main()
{
	vector<unsigned> topology;
	topology.push_back(3);
	topology.push_back(2);
	topology.push_back(1);
	Net myNet(topology);
	
	vector<double> inputVals;
	myNet.feedForward(inputVals);

	vector<double> targetVals;
	myNet.backProp(targetVals);

	vector<double> resultVals;
	myNet.getResults(resultVals);	
	
}
