
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <vector>
#include <truth_table.h>


#include "../lib/libblif/interface.h"

#include "interface.h"
#include "pattern_gen.h"
#include "../newApprox/src/blif_builder.h"


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

    FILE *fp;
    fp = fopen(file.c_str(), "r");
    if (fp == NULL) {
        std::cerr << file << std::endl;
        assert(0);
    }
    net = NULL;
    net = Bnet_ReadNetwork(fp);
    if (net == NULL) assert(0);

    assert(net->noutputs == net->npos);
    assert(net->ninputs == net->npis);

    //prepareDepths();

    fclose(fp);
}



void BlifBooleanNet::prepareBDD() const{
    if (ddmanager.isValid()) return;
    std::cout << "Building BDD... This could take a while." << std::endl;
    DdManager* ddm =
            Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
    if (ddm == NULL) exit(-2);
    cudd_build_v2(net, &ddm, filename.c_str(), BNET_GLOBAL_DD);
    ddmanager.setData(ddm);
    std::cout << "Done! BDD build complete." << std::endl;
}


size_t BlifBooleanNet::nInputs() const {
    return net->ninputs;
}

BlifBooleanNet::~BlifBooleanNet() {
    if (ddmanager.isValid()) {
        Bnet_FreeNetwork_Bdd(net, ddmanager.get());
        Cudd_Quit(ddmanager.get());
    } else {
        Bnet_FreeNetwork(net);
    }
    releaseSimulationContext();
}

TruthTable BlifBooleanNet::truthTable() const {
    prepareBDD();
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
                (Cudd_ReadOne(ddmanager.get()) == Cudd_Eval(ddmanager.get(), ddnode_pt, ivec));
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
    prepareBDD();
    if (v.size() != net->ninputs)
        assert(0);
    BnetNode *auxnd;
    st_lookup(net->hash,net->outputs[0],&auxnd);
    DdNode *ddnode_pt = auxnd->dd;
    int n = (Cudd_ReadOne(ddmanager.get())
             == Cudd_Eval(ddmanager.get(), ddnode_pt, (int*)(v.data()) ));
    return n;
}

const std::string &BlifBooleanNet::name() const {
    return filename;
}

size_t BlifBooleanNet::nodeCount() const {
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

size_t BlifBooleanNet::gateCount() const {
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

size_t BlifBooleanNet::nOutputs() const {
    return net->noutputs;
}

const vector<string> & BlifBooleanNet::inputNodeList() const {
    if (inputNodes.isValid())
        return inputNodes.get();
    std::vector<std::string> s;
    for (int i = 0; i < net->ninputs; ++i) {
        s.push_back(net->inputs[i]);
    }
    inputNodes.setData(s);
    return inputNodes.get();
}

const vector<string> & BlifBooleanNet::outputNodeList() const {
    if (outputNodes.isValid())
        return outputNodes.get();
    std::vector<std::string> s;
    for (int i = 0; i < net->noutputs; ++i) {
        s.push_back(net->outputs[i]);
    }
    outputNodes.setData(s);
    return outputNodes.get();
}

const set<BlifBooleanNet::BnetNodeID> &BlifBooleanNet::internalNodeSet() const {
    if (internalNodes.isValid())
        return internalNodes.get();
    std::set<std::string> s;
    BnetNode *node = net->nodes;
    auto in = this->inputNodeSet();
    auto out = this->outputNodeSet();
    while (node != nullptr) {
        if (in.count(node->name) != 0) {
            node = node->next;
            continue;
        }
        if (out.count(node->name) != 0) {
            node = node->next;
            continue;
        }
        s.insert(string(node->name));
        node = node->next;
    }
    internalNodes.setData(s);
    return internalNodes.get();
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
    prepareBDD();
    if (v.size() != net->ninputs)
        assert(0);
    std::vector<int> out;
    for (int i = 0; i < net->noutputs; ++i) {
        BnetNode *auxnd;
        st_lookup(net->hash, net->outputs[i], &auxnd);
        DdNode *ddnode_pt = auxnd->dd;
        int n = (Cudd_ReadOne(ddmanager.get())
                 == Cudd_Eval(ddmanager.get(), ddnode_pt, (int *) (v.data())));
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
    ofile << "ratio = .5;" << "\n";
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
    if (n == nullptr) assert(0);
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
//    fout << "# inputDepth = " << ffc.depth2Input << endl;
//    fout << "# outputDepth = " << ffc.depth2Output << endl;
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

CircuitProfile BlifBooleanNet::profile(int samples) {
    assert(0);
    assert(samples >= 0);
    InfiniteRandomPatternGenerator g(this->nInputs());
    CircuitProfile p(this->name());
    const auto& nodes = this->totalNodeSet();
    for (int i = 0; i < samples; ++i) {
        for (const auto& node : nodes) {
            int ret = this->evalAt(g.generate(), node);
            if (ret)
                p.incTrue(node);
            else
                p.incFalse(node);
        }
    }
    return p;
}

const set<BlifBooleanNet::BnetNodeID> &BlifBooleanNet::inputNodeSet() const {
    if (this->inputNodesSet.isValid())
        return inputNodesSet.get();
    inputNodesSet.setData(
            std::set<string>(
                    this->inputNodeList().begin(),
                    this->inputNodeList().end()
            ));
    return inputNodesSet.get();
}

const set<BlifBooleanNet::BnetNodeID> &BlifBooleanNet::outputNodeSet() const {
    if (this->outputNodesSet.isValid())
        return outputNodesSet.get();
    outputNodesSet.setData(
            std::set<string>(
                    this->outputNodeList().begin(),
                    this->outputNodeList().end()
            ));
    return outputNodesSet.get();
}

void
BlifBooleanNet::exportPartialBlif(const std::string &fname,
                                  set<BlifBooleanNet::BnetNodeID> omits) const {
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
        if (omits.count(nd->name)) continue;
        if (nd->type != BNET_INPUT_NODE
            && nd->type != BNET_PRESENT_STATE_NODE) {
            fprintf(fp,".names");
            for (i = 0; i < nd->ninp; i++) {
                (void) fprintf(fp," %s",nd->inputs[i]);
            }
            fprintf(fp," %s\n",nd->name);
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


void BlifBooleanNet::exportReplacedBlif(const std::string &fname,
                                        const BlifBooleanNet::FFC &ffc,
                                        const BlifBuilder &blifBuilder) {
    assert(std::is_permutation(
                   ffc.inputNode.begin(), ffc.inputNode.end(),
                   blifBuilder.inputName().begin()));
    assert(ffc.name == blifBuilder.outputNode());

    this->exportPartialBlif(fname, ffc.nodeSet);

    ofstream file(fname);

    file << blifBuilder.exportBlifBody();
}


