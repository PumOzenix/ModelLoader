#include "Model.h"
#include <vector>
Model::Model(string FileName) { //Model class constructor
	LoadModel(FileName);
}
Model::~Model() {
	//Clears all the vector lists from memory
	Vertices.clear();
	Materials.clear();
	Cells.clear();
	Pyramids.clear();
	Tetrahedrons.clear();
	Hexahedrons.clear();
}
int Model::SaveModel(void) {
	return 0;
}
void Model::LoadModel(string FileName) { //Loads the model
	string line, data;
	int LineNum = 0; //Initialise variables
	ifstream file;

	file.open(FileName); //Loads the model file 
	if (file.is_open()) { //Checks the file is open 
		while (getline(file, data)) { //Read each line and send data to classes
			LineNum++; //Increment current line
			line = data[0]; //First char for each line
	
			if (data.empty() || line == "#") continue; //Checks if the data is empty or a comment (Ignore)
			
			//If statement to check which type of data is being read
			if (line == "v")
				SetVertices(data); //Sends data to vector class
			else if (line == "m")
				SetMaterial(data); //Sends data to material class
			else if (line == "c")
				SetCell(data); //Sends data to cell class
			else
				throw "Unknown type: " + line + ". Fix model file"; //Unknown letter found
		}
		file.close(); // Close the file when finished
	}
	else {
		throw "Error opening file, use a compatible model file";
		exit(EXIT_FAILURE);

	}
		

}
void Model::SetCell(string data) {
	istringstream iss(data); //string stream to read characters in string
	vector<std::string> results((std::istream_iterator<std::string>(iss)), //Splits into characters
		istream_iterator<std::string>());

	int ID = stoi(results[1]); //Gather the ID from second value in results string
	string Type = results[2]; //Reads the corresponding char type from the string 
	int MaterialID = stoi(results[3]);

	//Checks to see which type of shape it is Hexahedron/Pyramid/Tetrahedron
	if (Type == "h") {
		Hexahedron *H = new Hexahedron; //Creates a new hexahedron object 
		H->setCell(ID, MaterialID, Type); //Sets the data for the new cell
		
		//Iterates through all vertices for the cell and sends them to the class
		for (int i = 0; i < 8; i++) {
			H->setVertices(stoi(results[i + 4])); //Set the vertex from the resulsts data stream 
		}
		Hexahedrons.push_back(*H); //Add this object to the vector array so it can be used in the future.
	} 
	else if (Type == "p") { //Shape type pyramid 
		Pyramid *P = new Pyramid;
		P->setCell(ID, MaterialID, Type); // Set the cell data 
		
		for (int i = 0; i < 5; i++) {	
			P->setVertices(stoi(results[i + 4]));
		}
		Pyramids.push_back(*P);//Stored the data into the vector array so it can be reference in the future
	}
	else if (Type == "t") { //Same as the if statemnets above but for the shape tetrahedron.
		Tetrahedron *T = new Tetrahedron;
		T->setCell(ID, MaterialID, Type);
		
		for (int i = 0; i < 4; i++) {		
			T->setVertices(stoi(results[i + 4]));
		}	
		Tetrahedrons.push_back(*T);
	}

}
void Model::SetVertices(string data) { //creates new vector objects then sends the data to the new vector class created above 

	//~~Reads thespecific data from the string using index~~
	istringstream iss(data);
	vector<std::string> results((std::istream_iterator<std::string>(iss)),
		istream_iterator<std::string>());
	int ID = stoi(results[1]);
	float X = stof(results[2]);
	float Y = stof(results[3]);
	float Z = stof(results[4]);

	Vector *V = new Vector; //Creates a new vector object
	V->SetVector(ID, X, Y, Z); //Sends the data to the class function
	Vertices.push_back(*V); //Adds it to the vector list 
}
void Model::SetMaterial(string data) {
	//Sends the data to a string stream so that it can be used to find the  data items for the class
	istringstream iss(data);
	vector<string> results((istream_iterator<string>(iss)),
		istream_iterator<string>());

	int ID = stoi(results[1]); //Uses index to find data such as ID within the line of data
	int Density = stoi(results[2]);
	string Name = results[4];
	string Colour = results[3];

	Material *M = new Material; //Creates the object and initialises it with the data 
	M->setMaterial(ID, Density, Colour, Name);
	Materials.push_back(*M);
}
int Model::GetCell(int ID, string Type) { //Used to get a specific cell
	if (Type == "p") { //Checks which type of shape it is 
		if (ID <= Pyramids.size())
			return Pyramids[ID].getCellID();
	}
	else if (Type == "t") {
		if (ID <= Tetrahedrons.size())
			return Tetrahedrons[ID].getCellID();

	}
	else if (Type == "h") {
		if (ID <= Pyramids.size())
			return Pyramids[ID].getCellID();

	}
	else
		cout << "Type not Found" << endl;
		
}
void Model::GetVertices(int ID) {
	float x = Vertices[ID].getx(); //Gets the x,y and z for a vertex with an id of ID
	float y = Vertices[ID].gety();
	float z = Vertices[ID].getz();

	this->x = x; //Sets the variables to be used
	this->y = y;
	this->z = z;

}
void Model::GetMaterial(int ID) { //Gets the material data from the material class with id ID
	int ID2 = Materials[ID].getMatID();
	string Colour = Materials[ID].getColour();
	string Name = Materials[ID].getName();
	int Density = Materials[ID].getDensity();

	cout << "ID: "<<ID2 << "Colour: "<< Colour << "Name: "<<Name <<"Density"<< Density;
	//Prints it to the screen 

}
vector<float> Model::FindCentre(void) { //Finds the centre of the model 
	Vector sum;
	sum.SetVector(0,0.0, 0.0, 0.0);
	for (int Vertex = 0; Vertex < Vertices.size(); Vertex++) {
		sum = sum.operator+(Vertices[Vertex]); //Adds all the vectors together
	}
	
	sum = sum.divide((float)Vertices.size()); //Divides by the number of vectors
	vector<float> output = { sum.getx(),sum.gety(),sum.getz() };
	return output;


}
int Model::NumberVertices(void) {
	return Vertices.size();

}
vector<int> Model::NumberCells(void) {
	int NumP = Pyramids.size();
	int NumT = Tetrahedrons.size();
	int NumH = Hexahedrons.size();

	
	vector<int> output = { NumP,NumT,NumH };
	return output;
}
double Model::GetCellVolume(int ID) { //Get the volume of the cell
	Hexahedrons[ID].setV(Vertices); //Send the required vertices
	return Hexahedrons[ID].getvolume(); //Get volume
}
