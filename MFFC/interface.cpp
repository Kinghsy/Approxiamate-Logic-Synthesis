#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "../lib/libblif/bnet.h"

#include <truth_table.h>
#include "interface.h"
#include "../lib/libblif/cudd_build_v2.h"

using namespace std;

typedef long long unsigned int ulli;
ulli power2(int power);
void getBits(ulli n, int* vec, int digit);

/*
string getMFFC(string infile, int minInput, int maxInput) {
    FILE *fp;
    fp = fopen(infile.c_str(), "r");
    BnetNetwork *net = Bnet_ReadNetwork(fp);
    if (net == NULL) {
        cout << "Syntax error in " << infile << endl;
        exit(2);
    }
    fclose(fp);

    stringstream s;
    searchMFFC(net, s, minInput, maxInput);
    Bnet_FreeNetwork(net);
    return s.str();
}
*/

BlifBooleanNet::BlifBooleanNet(const std::string &file) {
    filename = file;

    ddmanager = NULL;		/* pointer to DD manager */
    FILE *fp;
    fp = fopen(file.c_str(), "r");
    if (fp == NULL) assert(0);
    net = Bnet_ReadNetwork(fp);

    ddmanager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
    if (ddmanager == NULL) exit(-2);
    cudd_build_v2(net, &ddmanager, file.c_str(), BNET_GLOBAL_DD);

    assert(net->noutputs == net->npos);
    assert(net->ninputs == net->npis);

    prepareDepths();

    fclose(fp);
}

int BlifBooleanNet::nInputs() const {
    return net->ninputs;
}

BlifBooleanNet::~BlifBooleanNet() {
    Bnet_FreeNetwork_Bdd(net, ddmanager);
    Cudd_Quit(ddmanager);
}

TruthTable BlifBooleanNet::truthTable() const {
    BnetNode *auxnd;
    st_lookup(net->hash,net->outputs[0],&auxnd);
    DdNode *ddnode_pt = auxnd->dd;

    int nInputs = net->ninputs;
    vector<int> out;

    ulli max = power2(nInputs);
    int *ivec = new int[nInputs];
    for (ulli i = 0; i < max ; ++i) {
        getBits(i, ivec, nInputs);
        int n =
                (Cudd_ReadOne(ddmanager) == Cudd_Eval(ddmanager, ddnode_pt, ivec));
        out.push_back(n);
    }
    delete[] ivec;

    TruthTable table(nInputs, out);
    for (int j = 0; j < nInputs; ++j) {
        table.getName(j) = net->inputs[j];
    }
    table.outName = auxnd->name;

    return table;
}

int BlifBooleanNet::evalAt(const std::vector<int> &v,
                           const std::string& name) const {
    if (v.size() != net->ninputs)
        assert(0);
    BnetNode *auxnd;
    st_lookup(net->hash,net->outputs[0],&auxnd);
    DdNode *ddnode_pt = auxnd->dd;
    int n = (Cudd_ReadOne(ddmanager)
             == Cudd_Eval(ddmanager, ddnode_pt, (int*)(v.data()) ));
    return n;
}

const std::string &BlifBooleanNet::name() const {
    return filename;
}

int BlifBooleanNet::nodeCount() const {
    if (nNode.isValid())
        return nNode.get();
    int count = 0;
    BnetNode *n = net->nodes;
    while (n != NULL) {
        count++;
        n = n->next;
    }
    nNode.setData(count);
    return count;
}

int BlifBooleanNet::gateCount() const {
    if (nGates.isValid())
        return nGates.get();
    int c = nodeCount();
    int n = c - net->npis ;
    nGates.setData(n >= 0 ? n : 0);
    return nGates.get();
}

const std::string BlifBooleanNet::netName() const {
    return net->name;
}

/*
BlifBooleanNet BlifBooleanNet::getMFFC(int minInput,
                                       int maxInput) const {
    assert(minInput >= 0);
    assert(maxInput >= minInput);
    std::string fname = std::string("mffc.blif");
    ofstream fs(fname);
    searchMFFC(net, fs, minInput, maxInput);
    fs.close();
    return BlifBooleanNet(fname);
}
*/

int BlifBooleanNet::nOutputs() const {
    return net->noutputs;
}

const set<string> & BlifBooleanNet::inputNodeSet() const {
    if (inputNodes.isValid())
        return inputNodes.get();
    std::set<std::string> s;
    for (int i = 0; i < net->ninputs; ++i) {
        s.insert(net->inputs[i]);
    }
    inputNodes.setData(s);
    return inputNodes.get();
}

const set<string> & BlifBooleanNet::outputNodeSet() const {
    if (outputNodes.isValid())
        return outputNodes.get();
    std::set<std::string> s;
    for (int i = 0; i < net->noutputs; ++i) {
        s.insert(net->outputs[i]);
    }
    outputNodes.setData(s);
    return outputNodes.get();
}

const set<string> & BlifBooleanNet::totalNodeSet() const {
    if (totalNodes.isValid())
        return totalNodes.get();
    std::set<std::string> s;
    BnetNode *node = net->nodes;
    while (node != NULL) {
        s.insert(node->name);
        node = node->next;
    }
    totalNodes.setData(s);
    return totalNodes.get();
}

std::vector<int> BlifBooleanNet::evalAllOutputAt(const std::vector<int> &v) const {
    if (v.size() != net->ninputs)
        assert(0);
    std::vector<int> out;
    for (int i = 0; i < net->noutputs; ++i) {
        BnetNode *auxnd;
        st_lookup(net->hash, net->outputs[i], &auxnd);
        DdNode *ddnode_pt = auxnd->dd;
        int n = (Cudd_ReadOne(ddmanager)
                 == Cudd_Eval(ddmanager, ddnode_pt, (int *) (v.data())));
        out.push_back(n);
    }
    return out;
}

void BlifBooleanNet::exportBlifToFile(const std::string &fname) const {
    FILE* fp = fopen(fname.c_str(), "w+");
    BnetNode *nd;
    BnetTabline *tl;
    int i;

    if (net == NULL) return;

    (void) fprintf(fp,".model %s\n", net->name);
    (void) fprintf(fp,".inputs ");
    for(i = 0; i < net->npis; i++)
        fprintf(fp, "%s ", net->inputs[i]);
    fprintf(fp, "\n");

    (void) fprintf(fp,".outputs ");
    for(i = 0; i < net->npos; i++)
        fprintf(fp, "%s ", net->outputs[i]);
    fprintf(fp, "\n");
    nd = net->nodes;
    while (nd != NULL) {
        if (nd->type != BNET_INPUT_NODE && nd->type != BNET_PRESENT_STATE_NODE) {
            (void) fprintf(fp,".names");
            for (i = 0; i < nd->ninp; i++) {
                (void) fprintf(fp," %s",nd->inputs[i]);
            }
            (void) fprintf(fp," %s\n",nd->name);
            tl = nd->f;
            while (tl != NULL) {
                if (tl->values != NULL) {
                    (void) fprintf(fp,"%s %d\n",tl->values,
                                   1 - nd->polarity);
                } else {
                    (void) fprintf(fp,"%d\n", 1 - nd->polarity);
                }
                tl = tl->next;
            }
        }
        nd = nd->next;
    }
    (void) fprintf(fp,".end\n");

    fclose(fp);
}

void BlifBooleanNet::exportGraphViz(const std::string &fname) const {
    ofstream ofile(fname);
    ofile << "digraph " << net->name << "{" << "\n";
    //ofile << "size =\"4,4\"" << "\n";
    ofile << "{" << "\n";
    ofile << "rank = source;" << "\n";
    for (int i = 0; i < net->ninputs; ++i) {
        ofile << net->inputs[i] << "  [shape=box,style=filled,color=\".7 .3 1.0\"];" << "\n";
    }
    ofile << "}" << "\n";
    ofile << "{" << "\n";
    ofile << "rank = sink;" << "\n";
    for (int i = 0; i < net->noutputs; ++i) {
        ofile << net->outputs[i] << "  [shape=invtriangle,style=filled,color=\".7 .3 1.0\"];" << "\n";
    }
    ofile << "}" << "\n";
    BnetNode *node = net->nodes;
    while (node != NULL) {
        for (int i = 0; i < node->ninp; ++i) {
            ofile << node->inputs[i] << " -> " << node->name << ";" << "\n";
        }
        node = node->next;
    }
    ofile << "}" << endl;
}

void BlifBooleanNet::exportGraphVizwithHighlight(
        const string& fname, const set<string >& HLNodes, const string color) const {
    ofstream ofile(fname);
    ofile << "digraph " << net->name << "{" << "\n";
    //source nodes
    ofile << "{" << "\n";
    ofile << "rank = source;" << "\n";
    for (int i = 0; i < net->ninputs; ++i) {
        ofile << net->inputs[i] << "  [shape=box,style=filled,color=green];" << "\n";
    }
    ofile << "}" << "\n";
    //sink nodes
    ofile << "{" << "\n";
    ofile << "rank = sink;" << "\n";
    for (int i = 0; i < net->noutputs; ++i) {
        ofile << net->outputs[i] << "  [shape=invtriangle,style=filled,color=purple];" << "\n";
    }
    ofile << "}" << "\n";
    //median nodes
    ofile << "{" << "\n";
    for (const auto &item : HLNodes) {
        ofile << item << "  [style=filled,color="<< color <<"];" << "\n";
    }
    ofile << "}" << "\n";

    BnetNode *node = net->nodes;
    while (node != NULL) {
        for (int i = 0; i < node->ninp; ++i) {
            ofile << node->inputs[i] << " -> " << node->name << " ";
            if ((HLNodes.find(node->name) != HLNodes.end())
                && (HLNodes.find(node->inputs[i]) != HLNodes.end()))
                ofile << "[color="<< color <<"]";
            ofile << ";" << "\n";
        }
        node = node->next;
    }
    ofile << "}" << endl;
}

BnetNode *BlifBooleanNet::getNodeByName(const std::string &name) const{
    BnetNode* n = nullptr;
    st_lookup(net->hash, (void*)name.c_str(), &n);
    return n;
}

void BlifBooleanNet::exportFfcToBlifFile(const BlifBooleanNet::FFC &ffc,
                                         const std::string &filename) const {
    ofstream fout(filename);
    if (!fout) assert(0);

    const std::string& cnode = ffc.name;
    const std::set<BnetNodeID>& ffcNode = ffc.nodeSet;

    const BnetNode *nd = getNodeByName(cnode);

    fout << "# ffc of " << ffc.name << "@" << net->name << endl;
    fout << "# inputDepth = " << ffc.depth2Input << endl;
    fout << "# outputDepth = " << ffc.depth2Output << endl;
    fout << "# number of inputs = " << ffc.inputNode.size() << endl;
    fout << "# total number of nodes = " << ffc.nodeSet.size() << endl;
    fout << "# all nodes: ";
    for(auto nodeId : ffc.totalSet) fout << nodeId << " ";
    fout << endl;
    fout << endl;
    fout << ".model bigNode" << endl;
    fout << ".inputs ";
    for(auto nodeId : ffc.inputNode) fout << nodeId  << " ";
    fout << endl;
    fout << ".outputs " << cnode << endl;

    for(auto itrs = ffcNode.begin(); itrs != ffcNode.end(); itrs++) {
        BnetNodeID node = *itrs;
        BnetNode *tmp = getNodeByName(node);
        if(tmp->type == BNET_INPUT_NODE) continue;
        fout << ".names ";
        for(int i = 0 ; i < tmp->ninp; i++)
            fout << tmp->inputs[i] << " ";
        fout << node << endl;
        BnetTabline *t = tmp->f;
        while(t != NULL) {
            if(t->values != NULL)
                fout << t->values << " " << 1 - tmp->polarity << endl;
            else
                fout << 1 - tmp->polarity << endl;
            t = t->next;
        }
    }
    fout << ".end" << endl;
}



ulli power2(int power) {
    return ((ulli)1) << power;
}

void getBits(ulli n, int* vec, int digit) {
    for (int i = 0; i < digit; ++i) {
        if ((n & power2(i)) > 0)
            vec[i] = 1;
        else
            vec[i] = 0;
    }
}
