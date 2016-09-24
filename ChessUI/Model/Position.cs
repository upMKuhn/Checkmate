using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChessUI.Model
{
    public struct Position : IComparable<Position>
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
            TryCorrectFileRank(ref file, ref rank);
            if (rank > 8 || rank < 1)
                throw new IndexOutOfRangeException("Rank larger than 8 || Smaller than 0");
            if (file > 8 || file < 1)
                throw new IndexOutOfRangeException("File larger than 8 || Smaller than 0");


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
            String fileChar = "ABCDEFGH";
            return fileChar[Rank - 1] + File.ToString();
        } }

        private static void TryCorrectFileRank(ref int file, ref int rank)
        {
            int plusRank = 0, plusFile = 0;

            if (rank > 8)
            {
                plusFile -= Math.Min(rank / 8, 1);
                plusRank += plusFile * 8;
            }
            else if (rank < 1)
            {
                plusFile -= Math.Abs(Math.Max(rank / 8, 1));
                rank = Math.Abs(rank + 8);
            }

            file += plusFile;
            rank += plusRank;
        }

        public int CompareTo(Position other)
        {
            return Index < other.Index
                ? -1
                : (Index == other.Index
                    ? 0
                    : 1);
        }

        public static bool operator ==(Position left, Position right)
        {
            if (object.ReferenceEquals(left, null) && object.ReferenceEquals(right, null))
                return true;
            if (object.ReferenceEquals(left, null) || object.ReferenceEquals(right, null))
                return false;
            return left.Index == right.Index;
        }

        public static bool operator !=(Position left, Position right)
        {
            if (object.ReferenceEquals(left, null) && object.ReferenceEquals(right, null))
                return true;
            if (object.ReferenceEquals(left, null) || object.ReferenceEquals(right, null))
                return false;
            return left.Index != right.Index;
        }

        public static bool operator <(Position p1, Position p2)
        {
            return p1.Index < p2.Index;
        }

        public static bool operator >(Position p1, Position p2)
        {
            return p1.Index > p2.Index;
        }

        public static bool operator <=(Position p1, Position p2)
        {
            return p1.Index <= p2.Index;
        }

        public static bool operator >=(Position p1, Position p2)
        {
            return p1.Index >= p2.Index;
        }
    }
}
