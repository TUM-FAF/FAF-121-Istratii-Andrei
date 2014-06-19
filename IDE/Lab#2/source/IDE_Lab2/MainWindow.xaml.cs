using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace IDE_Lab2
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public ProgressBar currentPB;

        public MainWindow()
        {
            InitializeComponent();
            currentPB = PBAR1;
            SLIDER.Value = 50;
        }


        private void SLIDER_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            currentPB.Value = SLIDER.Value;
        }

        private void BTN1_Click(object sender, RoutedEventArgs e)
        {
            currentPB = PBAR1;
            SLIDER.Value = currentPB.Value;
        }


        private void BTN2_Click(object sender, RoutedEventArgs e)
        {
            currentPB = PBAR2;
            SLIDER.Value = currentPB.Value;
        }

        private void BTN3_Click(object sender, RoutedEventArgs e)
        {
            currentPB = PBAR3;
            SLIDER.Value = currentPB.Value;
        }
    }
}
