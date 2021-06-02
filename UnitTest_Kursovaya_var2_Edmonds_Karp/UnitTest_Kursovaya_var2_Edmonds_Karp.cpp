// This program was made by Bannikov Nikita, group 9309 
#include "pch.h"
#include "CppUnitTest.h"
#include <stdexcept>
#include <windows.h>
#include "../Kursovaya_var2_Edmonds_Karp/Header.h"
#include "../Kursovaya_var2_Edmonds_Karp/Implementation.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace EdmondsKarp
{
	// Graphs verifications
	TEST_CLASS(MaxFlowVerification)
	{
	public:
		MaxFlowVerification() {
			Logger::WriteMessage(">> MaxFlowVerification, ");
			graph = new Graph;
		}

		~MaxFlowVerification() {
			Logger::WriteMessage(" >> ~MaxFlowVerification");
			delete graph;
		}

		TEST_METHOD(GraphForKusrosvaya)
		{
			graph->addEdge("S", "O", 3);
			graph->addEdge("S", "P", 3);
			graph->addEdge("O", "Q", 3);
			graph->addEdge("O", "P", 2);
			graph->addEdge("P", "R", 2);
			graph->addEdge("Q", "R", 4);
			graph->addEdge("Q", "T", 2);
			graph->addEdge("R", "T", 3);

			int max_flow = graph->maxFlow();
			Assert::AreEqual(max_flow, 5);
		}

		TEST_METHOD(GraphFromWikipedia)
		{
			// See https://en.wikipedia.org/wiki/Edmonds%E2%80%93Karp_algorithm
			// A -> S
			// G -> T
			graph->addEdge("S", "B", 3);
			graph->addEdge("S", "D", 3);

			graph->addEdge("B", "C", 4);

			graph->addEdge("C", "S", 3);
			graph->addEdge("C", "D", 1);
			graph->addEdge("C", "E", 2);
			
			graph->addEdge("E", "B", 1);
			graph->addEdge("D", "E", 2);
			graph->addEdge("D", "F", 6);

			graph->addEdge("F", "T", 9);
			graph->addEdge("E", "T", 1);

			int max_flow = graph->maxFlow();
			Assert::AreEqual(max_flow, 5);
		}

		TEST_METHOD(GraphFromInetFirst)
		{
			// See https://cp-algorithms.com/graph/edmonds_karp.html

			graph->addEdge("S", "A", 7);
			graph->addEdge("S", "D", 4);

			graph->addEdge("D", "A", 3);
			graph->addEdge("A", "B", 5);
			graph->addEdge("A", "C", 3);
			graph->addEdge("D", "C", 2);
			graph->addEdge("C", "B", 3);

			graph->addEdge("B", "T", 8);
			graph->addEdge("C", "T", 5);

			int max_flow = graph->maxFlow();
			Assert::AreEqual(max_flow, 10);
		}
	
		TEST_METHOD(GraphFromInetSecond)
		{
			// See https://prabhusiddarth.wordpress.com/2018/03/08/ford-fulkerson-algorithm-for-maximum-flow-problem/
			// 0 -> S
			// 5 -> T

			graph->addEdge("S", "1", 16);
			graph->addEdge("S", "2", 13);

			graph->addEdge("1", "2", 10);
			graph->addEdge("2", "1", 4);
			graph->addEdge("1", "3", 12);
			graph->addEdge("3", "2", 9);
			graph->addEdge("2", "4", 14);
			graph->addEdge("4", "3", 7);

			graph->addEdge("3", "T", 20);
			graph->addEdge("4", "T", 4);

			int max_flow = graph->maxFlow();
			Assert::AreEqual(max_flow, 23);
		}
	

	private:
		// Keeps graph instance
		Graph* graph;
	};


	
	// Loading from file verifications
	TEST_CLASS(GraphFromFile)
	{
	public:
		GraphFromFile() {
			Logger::WriteMessage(">> GraphFromFile: ");
			graph = new Graph;
		}

		~GraphFromFile() {
			Logger::WriteMessage(" >> ~GraphFromFile");
			delete graph;
		}

		TEST_METHOD(GraphForKusrosvaya)
		{
			int max_flow = 0;
			try {
				graph->load_from_file("../valid_graph_data_1.txt");
				max_flow = graph->maxFlow();
			}
			catch (const std::exception& exc) {
				Logger::WriteMessage(exc.what());
			}
			Assert::AreEqual(5, max_flow);
		}

		TEST_METHOD(FileDoesNotExist)
		{
			bool exceptionThrown = false;
			try {
				graph->load_from_file("../file_does_not _exists.txt");
			}
			catch (...) {
				exceptionThrown = true;
			}
			Assert::AreEqual(true, exceptionThrown);
		}

		TEST_METHOD(InvalidFileFormatFirst)
		{
			bool exceptionThrown = false;
			try {
				graph->load_from_file("../invalid_graph_data_1.txt");
			}
			catch (const std::exception& exc) {
				Logger::WriteMessage(exc.what());
				exceptionThrown = true;
			}
			Assert::AreEqual(true, exceptionThrown);
		}

		TEST_METHOD(InvalidFileFormatSecond)
		{
			bool exceptionThrown = false;
			try {
				graph->load_from_file("../invalid_graph_data_2.txt");
			}
			catch (const std::exception& exc) {
				Logger::WriteMessage(exc.what());
				exceptionThrown = true;
			}
			Assert::AreEqual(true, exceptionThrown);
		}

	private:
		// Keeps graph instance
		Graph* graph;

		std::string workingdir(const string& file_name)
		{
			char buf[256];
			GetCurrentDirectoryA(2048, buf);
			return std::string(buf) + '\\' + file_name;
		}
	};


	// Methods verification
	TEST_CLASS(GraphMethods)
	{
	public:
		GraphMethods() {
			Logger::WriteMessage(">> GraphMethods, ");
			graph = new Graph;
		}

		~GraphMethods() {
			Logger::WriteMessage(" >> ~GraphMethods");
			delete graph;
		}

		TEST_METHOD(SimpleGraph)
		{
			int flow = 23;
			graph->addEdge("S", "T", flow);
			int max_flow = graph->maxFlow();
			Assert::AreEqual(flow, max_flow);
		}

		bool exceptionThrown = false;

		TEST_METHOD(NotDefinedS)
		{
			graph->addEdge("SOME", "T", 1);

			bool exceptionThrown = false;
			try {
				graph->maxFlow();
			} catch (...) {
				exceptionThrown = true;
			}

			Assert::AreEqual(exceptionThrown, true);
		}

		TEST_METHOD(NotDefinedT)
		{
			graph->addEdge("S", "SOME", 1);

			bool exceptionThrown = false;
			try {
				graph->maxFlow();
			}
			catch (...) {
				exceptionThrown = true;
			}

			Assert::AreEqual(exceptionThrown, true);
		}

		TEST_METHOD(PostitiveCapacity)
		{
			bool exceptionThrown = false;
			try {
				graph->addEdge("S", "SOME", -1);
			}
			catch (...) {
				exceptionThrown = true;
			}

			Assert::AreEqual(exceptionThrown, true);
		}

		TEST_METHOD(NotEmptyFromName)
		{
			bool exceptionThrown = false;
			try {
				graph->addEdge(" ", "SOME", 1);
			}
			catch (...) {
				exceptionThrown = true;
			}
			Assert::AreEqual(exceptionThrown, true);
		}

		TEST_METHOD(NotEmptyToName)
		{
			bool exceptionThrown = false;
			try {
				graph->addEdge("S", " ", 1);
			}
			catch (...) {
				exceptionThrown = true;
			}
			Assert::AreEqual(exceptionThrown, true);
		}

	private:
		// Keeps graph instance
		Graph* graph;
	};
}
