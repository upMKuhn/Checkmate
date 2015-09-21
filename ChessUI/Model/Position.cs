using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChessUI.Model
{
    public class Position
    {
        public static int RankFileToIndex(char rank, char file)
        {
            int intRank = 0;
            int intFile;
            char[] ranks = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };
            while (ranks[intRank] != rank && intRank < ranks.Length) { intRank++; }
            if (intRank > 8) { throw new Exception("Invalid Rank given"); }
            intFile = (int)Char.GetNumericValue(file);
            return (intFile - 1) * 8 + intRank;
        }

        public Position(int file, int rank)
        {
            File = file;
            Rank = rank;
        }

        public int File { get; set; }
        public int Rank { get; set; }
        public int Index
        {get
            {
                int pos;
                pos = (File - 1) * 8 + Rank - 1;
                return pos;
            }
        }

        public bool isSquareBlack
        { get {
                int modifyIndex = 0;
                if (Rank % 2 == 0)
                {
                    modifyIndex = -1;
                }
                return (File + modifyIndex) % 2 != 0;
        } }

        public string SquareName { get {
            char[] fileChar = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' };
            return fileChar[File-1] + Rank.ToString();
        } }
    }
}
