#include <iostream>
#include "TriangularMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"
#include <map>
#include <cmath>

using namespace std;
using namespace Eigen;
using namespace TriangularLibrary;

int main()
{
    TriangularMesh mesh;

    if(!ImportMesh(mesh))
    {
        cerr << "file not found" << endl;
        return 1;
    }

    /// Per visualizzare online le mesh:
    /// 1. Convertire i file .inp in file .vtu con https://meshconverter.it/it
    /// 2. Caricare il file .vtu su https://kitware.github.io/glance/app/

    Gedim::UCDUtilities utilities;
    {
        vector<Gedim::UCDProperty<double>> cell0Ds_properties(1);

        cell0Ds_properties[0].Label = "Marker";
        cell0Ds_properties[0].UnitLabel = "-";
        cell0Ds_properties[0].NumComponents = 1;

        vector<double> cell0Ds_marker(mesh.NumCell0Ds, 0.0);
        for(const auto &m : mesh.MarkerCell0Ds)
            for(const unsigned int id: m.second)
                cell0Ds_marker.at(id) = m.first;

        cell0Ds_properties[0].Data = cell0Ds_marker.data();

        utilities.ExportPoints("./Cell0Ds.inp",
                               mesh.Cell0DsCoordinates,
                               cell0Ds_properties);
    }

    {

        vector<Gedim::UCDProperty<double>> cell1Ds_properties(1);

        cell1Ds_properties[0].Label = "Marker";
        cell1Ds_properties[0].UnitLabel = "-";
        cell1Ds_properties[0].NumComponents = 1;

        vector<double> cell1Ds_marker(mesh.NumCell1Ds, 0.0);
        for(const auto &m : mesh.MarkerCell1Ds)
            for(const unsigned int id: m.second)
                cell1Ds_marker.at(id) = m.first;

        cell1Ds_properties[0].Data = cell1Ds_marker.data();

        utilities.ExportSegments("./Cell1Ds.inp",
                                 mesh.Cell0DsCoordinates,
                                 mesh.Cell1DsExtrema,
                                 {},
                                 cell1Ds_properties);
    }


    /* VERIFICO QUALI MARKER SONO STATI SALVATI */
    cout<<"MARKER0Ds"<<endl;
    map<unsigned int, list<unsigned int>>& m0 = mesh.MarkerCell0Ds;
    for(auto& [marker, list_id] : m0)
     {
        cout<<"Marker0D: "<<marker<<"  ID: ";
        for(auto& id : list_id)
            cout<<" "<<id<<", ";
        cout<<endl;
     }
    cout<<endl;
    cout<<endl;

    cout<<"MARKER1Ds"<<endl;
    map<unsigned int, list<unsigned int>>& m1 = mesh.MarkerCell1Ds;
    for(auto& [marker, list_id] : m1)
     {
        cout<<"Marker1D: "<<marker<<"  ID: ";
        for(auto& id : list_id)
            cout<<" "<<id<<", ";
        cout<<endl;
     }
    
     cout<<endl;
     cout<<endl;

     cout<<"Lunghezza segmenti:"<<endl;

    /*VERIFICO CHE TUTTI I SEGMENTI ABBIANO LUNGHEZZA NON NULLA*/
    MatrixXi m = mesh.Cell1DsExtrema;
    MatrixXd coordinates = mesh.Cell0DsCoordinates;
    for(unsigned int i = 0; i < m.cols(); i++){
        int a = m(0,i);
        int b = m(1,i);
        double x_1 = coordinates(0,a);
        double y_1 = coordinates(1,a);
        double x_2 = coordinates(0,b);
        double y_2 = coordinates(1,b);
        double d = sqrt((x_1-x_2)*(x_1-x_2) + (y_1-y_2)*(y_1-y_2));
        if (d > 1e-8){
            cout<<"Segment "<<i<<" OK! "<<endl;
        }else{
            cout<<"Segment "<<i<<" NOT OK!"<<endl;
        }
    }
    vector<vector<unsigned int>> v = mesh.Cell2DsVertices;
    

    /*VERIFICO CHE TUTTI I POLIGONI CREATI ABBIANO AREA NON NULLA*/
    unsigned int x = mesh.NumCell2Ds;
    for (unsigned int i = 0; i < x; i++){
        vector<unsigned int> vertices = v[i];
        unsigned int starter = vertices[0];
        double area = 0;
        for (unsigned int j = 1; j < vertices.size()-1; j++){
            Vector2d v;
            Vector2d w;
            v << coordinates(0,vertices[j]) - coordinates(0, vertices[0]), coordinates(1, vertices[j]) - coordinates(1, vertices[0]);
            w << coordinates(0, vertices[j+1]) - coordinates(0, vertices[0]), coordinates(1, vertices[j+1]) - coordinates(1, vertices[0]);
            area = area + areaTriangle(v,w);
        }
        if (area > 1e-8){
            cout <<"Polygon" <<i<<" OK!"<<endl;
            cout<<"Area of Polygon"<<i<<": "<<area<<endl;
        }else{
            cout <<"Polygon" <<i<<" NOT OK!"<<endl;
        }
    }

    return 0;
}
