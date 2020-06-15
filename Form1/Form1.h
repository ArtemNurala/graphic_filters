#pragma once

#include "ImageData.h"

namespace Form1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Drawing::Drawing2D;
	using namespace Threading::Tasks;

	/// <summary>
	/// Сводка для Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	private:
		BufferedGraphicsContext^  textGraph;
		BufferedGraphics^  bufMain, ^bufHistogr, ^bufProgress;
		Graphics^  grMain, ^grHistogr, ^grProgress;
		Rectangle^  rect;
		ImageData^  image;

	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private:
		System::Windows::Forms::Panel^  panelMain;
		System::Windows::Forms::Panel^  panelHistogr;
		System::Windows::Forms::MenuStrip^  menuStrip1;
		System::Windows::Forms::ToolStripMenuItem^  изображениеToolStripMenuItem;
		System::Windows::Forms::ToolStripMenuItem^  оригиналToolStripMenuItem;
		System::Windows::Forms::GroupBox^  groupBoxEdit;
		System::Windows::Forms::Label^  label1;
		System::Windows::Forms::Button^  button1;
		System::Windows::Forms::Button^  button2;
		System::Windows::Forms::Button^  button3;
		System::Windows::Forms::Button^  button4;
		System::Windows::Forms::Button^  button5;
		System::Windows::Forms::Button^  button6;
		System::Windows::Forms::ProgressBar^  progressBarOptimization;
		System::Windows::Forms::NumericUpDown^ numericAperture;
		System::Windows::Forms::TrackBar^ tracBarRes;
	private:
		/// <summary>
		/// Требуется переменная конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Обязательный метод для поддержки конструктора - не изменяйте
		/// содержимое данного метода при помощи редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->panelMain = (gcnew System::Windows::Forms::Panel());
			this->panelHistogr = (gcnew System::Windows::Forms::Panel());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->изображениеToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->оригиналToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->groupBoxEdit = (gcnew System::Windows::Forms::GroupBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->progressBarOptimization = (gcnew System::Windows::Forms::ProgressBar());
			this->numericAperture = (gcnew System::Windows::Forms::NumericUpDown());
			this->tracBarRes = (gcnew System::Windows::Forms::TrackBar());
			this->menuStrip1->SuspendLayout();
			this->groupBoxEdit->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericAperture))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->tracBarRes))->BeginInit();
			this->SuspendLayout();
			// 
			// panelMain
			// 
			this->panelMain->Location = System::Drawing::Point(300, 0);
			this->panelMain->Name = L"panelMain";
			this->panelMain->Size = System::Drawing::Size(1020, 680);
			this->panelMain->TabIndex = 0;
			// 
			// panelHistogr
			// 
			this->panelHistogr->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->panelHistogr->Location = System::Drawing::Point(10, 488);
			this->panelHistogr->Name = L"panelHistogr";
			this->panelHistogr->Size = System::Drawing::Size(256, 192);
			this->panelHistogr->TabIndex = 1;
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->изображениеToolStripMenuItem, 
				this->оригиналToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(1320, 24);
			this->menuStrip1->TabIndex = 2;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// изображениеToolStripMenuItem
			// 
			this->изображениеToolStripMenuItem->Name = L"изображениеToolStripMenuItem";
			this->изображениеToolStripMenuItem->Size = System::Drawing::Size(95, 20);
			this->изображениеToolStripMenuItem->Text = L"Изображение";
			this->изображениеToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::изображениеToolStripMenuItem_Click);
			// 
			// оригиналToolStripMenuItem
			// 
			this->оригиналToolStripMenuItem->Name = L"оригиналToolStripMenuItem";
			this->оригиналToolStripMenuItem->Size = System::Drawing::Size(74, 20);
			this->оригиналToolStripMenuItem->Text = L"Оригинал";
			this->оригиналToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::оригиналToolStripMenuItem_Click);
			// 
			// groupBoxEdit
			// 
			this->groupBoxEdit->Controls->Add(this->button6);
			this->groupBoxEdit->Controls->Add(this->button1);
			this->groupBoxEdit->Controls->Add(this->button2);
			this->groupBoxEdit->Controls->Add(this->button3);
			this->groupBoxEdit->Controls->Add(this->button5);
			this->groupBoxEdit->Location = System::Drawing::Point(10, 27);
			this->groupBoxEdit->Name = L"groupBoxEdit";
			this->groupBoxEdit->Size = System::Drawing::Size(217, 285);
			this->groupBoxEdit->TabIndex = 3;
			this->groupBoxEdit->TabStop = false;
			this->groupBoxEdit->Text = L"Фильтры";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(6, 19);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(205, 46);
			this->button1->TabIndex = 5;
			this->button1->Text = L"Искусственное наложение";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(6, 71);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(205, 46);
			this->button2->TabIndex = 6;
			this->button2->Text = L"Равномерное";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(6, 123);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(205, 46);
			this->button3->TabIndex = 7;
			this->button3->Text = L"Фильтр Гаусса";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(6, 175);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(205, 46);
			this->button5->TabIndex = 9;
			this->button5->Text = L"Тиснение";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &Form1::button5_Click);
			// 
			// button6
			// 
			this->button6->Location = System::Drawing::Point(6, 227);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(205, 46);
			this->button6->TabIndex = 10;
			this->button6->Text = L"Акварелизация";
			this->button6->UseVisualStyleBackColor = true;
			this->button6->Click += gcnew System::EventHandler(this, &Form1::button6_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(7, 315);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(54, 13);
			this->label1->TabIndex = 4;
			this->label1->Text = L"Апертура";
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(10, 357);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(205, 46);
			this->button4->TabIndex = 8;
			this->button4->Text = L"Резкость";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &Form1::button4_Click);
			// 
			// progressBarOptimization
			// 
			this->progressBarOptimization->Location = System::Drawing::Point(10, 460);
			this->progressBarOptimization->Name = L"progressBarOptimization";
			this->progressBarOptimization->Size = System::Drawing::Size(215, 22);
			this->progressBarOptimization->TabIndex = 10;
			this->progressBarOptimization->Visible = true;
			// 
			// numericAperture
			// 
			this->numericAperture->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) {2, 0, 0, 0});
			this->numericAperture->Location = System::Drawing::Point(10, 331);
			this->numericAperture->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {9, 0, 0, 0});
			this->numericAperture->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {3, 0, 0, 0});
			this->numericAperture->Name = L"numericAperture";
			this->numericAperture->Size = System::Drawing::Size(194, 20);
			this->numericAperture->TabIndex = 11;
			this->numericAperture->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {3, 0, 0, 0});
			// 
			// tracBarRes
			// 
			this->tracBarRes->Location = System::Drawing::Point(10, 409);
			this->tracBarRes->Minimum = 1;
			this->tracBarRes->Name = L"tracBarRes";
			this->tracBarRes->Size = System::Drawing::Size(220, 45);
			this->tracBarRes->TabIndex = 12;
			this->tracBarRes->Value = 1;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1320, 680);
			this->Controls->Add(this->groupBoxEdit);
			this->Controls->Add(this->panelMain);
			this->Controls->Add(this->panelHistogr);
			this->Controls->Add(this->menuStrip1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->progressBarOptimization);
			this->Controls->Add(this->numericAperture);
			this->Controls->Add(this->tracBarRes);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"Form1";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->groupBoxEdit->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericAperture))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->tracBarRes))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private:
		System::Void изображениеToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		{
			System::Windows::Forms::OpenFileDialog^  res = gcnew OpenFileDialog();
			res->Filter = "Image Files|*.jpg;*.jpeg;*.png";

			if (res->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				String^  filePath = res->FileName;

				image = gcnew ImageData(filePath, bufMain->Graphics, bufHistogr->Graphics, progressBarOptimization);
				image->Draw();
				groupBoxEdit->Enabled = true;
				bufMain->Render(grMain);
				bufHistogr->Render(grHistogr);
				bufProgress->Render(grProgress);
			}
		}

		System::Void оригиналToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		{
			image->Original();
			bufMain->Render(grMain);
			bufHistogr->Render(grHistogr);
			bufProgress->Render(grProgress);
		}

		System::Void button1_Click(System::Object^ sender, System::EventArgs^  e)
		{
			image->Noise();
			bufMain->Render(grMain);
			bufHistogr->Render(grHistogr);
			bufProgress->Render(grProgress);
		}

		System::Void button2_Click(System::Object^ sender, System::EventArgs^  e)
		{
			image->UniformNoiseReduction((int)numericAperture->Value, true);
			bufMain->Render(grMain);
			bufHistogr->Render(grHistogr);
			bufProgress->Render(grProgress);
		}

		System::Void button3_Click(System::Object^  sender, System::EventArgs^  e)
		{
			image->Gauss((int)numericAperture->Value, true);
			bufMain->Render(grMain);
			bufHistogr->Render(grHistogr);
			bufProgress->Render(grProgress);
		}

		System::Void button4_Click(System::Object^  sender, System::EventArgs^  e)
		{
			image->Focus((int)numericAperture->Value, (int)tracBarRes->Value, true);
			bufMain->Render(grMain);
			bufHistogr->Render(grHistogr);
			bufProgress->Render(grProgress);
		}

		System::Void button5_Click(System::Object^  sender, System::EventArgs^  e)
		{
			image->Binarize_Tisn((int)numericAperture->Value, false);
			//image->Binarize_Tisn((int)numericAperture->Value, false);
			bufMain->Render(grMain);
			bufHistogr->Render(grHistogr);
			bufProgress->Render(grProgress);
		}

		System::Void button6_Click(System::Object^  sender, System::EventArgs^  e)
		{
			image->Aquarel((int)numericAperture->Value, (int)tracBarRes->Value);
			bufMain->Render(grMain);
			bufHistogr->Render(grHistogr);
			bufProgress->Render(grProgress);
		}

		System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e)
		{
			textGraph = BufferedGraphicsManager::Current;
			textGraph->MaximumBuffer = System::Drawing::Size(panelMain->Width + 1, panelMain->Height + 1);
            bufMain = textGraph->Allocate(panelMain->CreateGraphics(), Rectangle(0, 0, panelMain->Width, panelMain->Height));
            bufMain->Graphics->Clear(Color::FromKnownColor(KnownColor::ControlDark));
            grMain = panelMain->CreateGraphics();
            textGraph = BufferedGraphicsManager::Current;
			textGraph->MaximumBuffer = System::Drawing::Size(panelHistogr->Width + 1, panelHistogr->Height + 1);
            bufHistogr = textGraph->Allocate(panelHistogr->CreateGraphics(), Rectangle(0, 0, panelHistogr->Width, panelHistogr->Height));
            bufHistogr->Graphics->Clear(Color::FromKnownColor(KnownColor::ControlDark));
            grHistogr = panelHistogr->CreateGraphics();
			textGraph = BufferedGraphicsManager::Current;
			textGraph->MaximumBuffer = System::Drawing::Size(progressBarOptimization->Width + 1, progressBarOptimization->Height + 1);
            bufProgress = textGraph->Allocate(progressBarOptimization->CreateGraphics(), Rectangle(0, 0, progressBarOptimization->Width, progressBarOptimization->Height));
            bufProgress->Graphics->Clear(Color::FromKnownColor(KnownColor::ControlDark));
            grProgress = progressBarOptimization->CreateGraphics();
		}
	};
}
