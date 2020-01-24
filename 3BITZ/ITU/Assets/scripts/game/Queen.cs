using UnityEngine;
using UnityEngine.UI;

public class Queen : BasePiece
{
    public override void Setup(Color newTeamColor, Color32 newSpriteColor, PieceManager newPieceManager)
    {
        base.Setup(newTeamColor, newSpriteColor, newPieceManager);

        mMovement = new Vector3Int(0, 0, 1);
        GetComponent<Image>().sprite = Resources.Load<Sprite>("T_Queen");
    }
    private bool MatchesState(int targetX, int targetY, CellState targetState)
    {
        CellState cellState = CellState.None;
        cellState = mCurrentCell.mBoard.ValidateCell(targetX, targetY, this);

        if (cellState == targetState)
        {
            mHighlightedCells.Add(mCurrentCell.mBoard.mAllCells[targetX, targetY]);
            return true;
        }
        return false;
    }

    private bool MatchesStateEnemy(int targetX, int targetY, CellState targetState, int col)
    {
        int curentX = mCurrentCell.mBoardPosition.x;
        int curentY = mCurrentCell.mBoardPosition.y;

        CellState cellState = CellState.None;
        CellState cellState2 = CellState.None;
        CellState cellState3 = CellState.None;
        cellState = mCurrentCell.mBoard.ValidateCell(targetX, targetY, this);

        if (col == 1 && mColor == Color.white)
        {
            cellState2 = mCurrentCell.mBoard.ValidateCell(targetX + mMovement.z, targetY + mMovement.z, this);
            if (cellState == targetState && cellState2 == CellState.Free)
            {
                mHighlightedCells.Add(mCurrentCell.mBoard.mAllCells[targetX + mMovement.z, targetY + mMovement.z]);
                return true;
            }
        }

        if (col == 1 && mColor == Color.black)
        {
            cellState2 = mCurrentCell.mBoard.ValidateCell(targetX + mMovement.z, targetY + mMovement.z, this);
            if (cellState == targetState && cellState2 == CellState.Free)
            {
                mHighlightedCells.Add(mCurrentCell.mBoard.mAllCells[targetX + mMovement.z, targetY + mMovement.z]);
                return true;
            }
        }



        if (col == 2 && mColor == Color.white)
        {
            cellState2 = mCurrentCell.mBoard.ValidateCell(targetX - mMovement.z, targetY + mMovement.z, this);

            if (cellState == targetState && cellState2 == CellState.Free)
            {

                mHighlightedCells.Add(mCurrentCell.mBoard.mAllCells[targetX - mMovement.z, targetY + mMovement.z]);
                return true;
            }
        }

        if (col == 2 && mColor == Color.black)
        {
            cellState2 = mCurrentCell.mBoard.ValidateCell(targetX - mMovement.z, targetY + mMovement.z, this);

            if (cellState == targetState && cellState2 == CellState.Free)
            {

                mHighlightedCells.Add(mCurrentCell.mBoard.mAllCells[targetX - mMovement.z, targetY + mMovement.z]);
                return true;
            }
        }


        if (col == 3 && mColor == Color.white)
        {
            cellState2 = mCurrentCell.mBoard.ValidateCell(targetX + mMovement.z, targetY - mMovement.z, this);
            if (cellState == targetState && cellState2 == CellState.Free)
            {
                mHighlightedCells.Add(mCurrentCell.mBoard.mAllCells[targetX + mMovement.z, targetY - mMovement.z]);
                return true;
            }
        }

        if (col == 3 && mColor == Color.black)
        {
            cellState2 = mCurrentCell.mBoard.ValidateCell(targetX + mMovement.z, targetY - mMovement.z, this);
            if (cellState == targetState && cellState2 == CellState.Free)
            {
                mHighlightedCells.Add(mCurrentCell.mBoard.mAllCells[targetX + mMovement.z, targetY - mMovement.z]);
                return true;
            }
        }

        if (col == 4 && mColor == Color.white)
        {
            cellState2 = mCurrentCell.mBoard.ValidateCell(targetX - mMovement.z, targetY - mMovement.z, this);

            if (cellState == targetState && cellState2 == CellState.Free)
            {

                mHighlightedCells.Add(mCurrentCell.mBoard.mAllCells[targetX - mMovement.z, targetY - mMovement.z]);
                return true;
            }
        }

        if (col == 4 && mColor == Color.black)
        {
            cellState2 = mCurrentCell.mBoard.ValidateCell(targetX - mMovement.z, targetY - mMovement.z, this);

            if (cellState == targetState && cellState2 == CellState.Free)
            {

                mHighlightedCells.Add(mCurrentCell.mBoard.mAllCells[targetX - mMovement.z, targetY - mMovement.z]);
                return true;
            }
        }

        return false;
    }

    protected override void CheckPathing()
    {
        int curentX = mCurrentCell.mBoardPosition.x;
        int curentY = mCurrentCell.mBoardPosition.y;

        CellState cellState = CellState.None;
        CellState cellState2 = CellState.None;
        CellState cellState3 = CellState.None;
        CellState cellState4 = CellState.None;

        int col = 0;

        //pohlad bieleho
        cellState = mCurrentCell.mBoard.ValidateCell(curentX + mMovement.z, curentY + mMovement.z, this); // v pravo hore
        cellState2 = mCurrentCell.mBoard.ValidateCell(curentX - mMovement.z, curentY + mMovement.z, this); // v lavo hore
        cellState3 = mCurrentCell.mBoard.ValidateCell(curentX + mMovement.z, curentY - mMovement.z, this); // v pravo dole
        cellState4 = mCurrentCell.mBoard.ValidateCell(curentX - mMovement.z, curentY - mMovement.z, this); //v lavo dole

        if (cellState == CellState.Free)
        {
            MatchesState(curentX + mMovement.z, curentY + mMovement.z, CellState.Free);
        }
        if (cellState == CellState.Enemy)
        {
            //vpravo
            col = 1;
            MatchesStateEnemy(curentX + mMovement.z, curentY + mMovement.z, CellState.Enemy, col);
        }

        if (cellState3 == CellState.Free)
        {
            MatchesState(curentX + mMovement.z, curentY - mMovement.z, CellState.Free);
        }
        if (cellState3 == CellState.Enemy)
        {
            //vpravo
            col = 3;
            MatchesStateEnemy(curentX + mMovement.z, curentY - mMovement.z, CellState.Enemy, col);
        }


        if (cellState2 == CellState.Free)
        {
            MatchesState(curentX - mMovement.z, curentY + mMovement.z, CellState.Free);
        }
        if (cellState2 == CellState.Enemy)
        {
            //vlavo
            col = 2;
            MatchesStateEnemy(curentX - mMovement.z, curentY + mMovement.z, CellState.Enemy, col);
        }

        if (cellState4 == CellState.Free)
        {
            MatchesState(curentX - mMovement.z, curentY - mMovement.z, CellState.Free);
        }
        if (cellState4 == CellState.Enemy)
        {
            //vlavo
            col = 4;
            MatchesStateEnemy(curentX - mMovement.z, curentY - mMovement.z, CellState.Enemy, col);
        }


    }
}
