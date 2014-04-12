import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class MineSweeperMaster_C {

	private static 		
	int cntbox[][] = {  {-1,-1}, {0,-1}, {1, -1},
						{-1, 0},         {1, 0},
						{-1, 1}, {0, 1}, {1, 1}};
	
	private int 	R, C, M;

	public MineSweeperMaster_C( BufferedReader in ) throws IOException{
		
		String read[] = in.readLine().split(" ");
		
		R = Integer.parseInt(read[0] );
		C = Integer.parseInt( read[1] );
		M = Integer.parseInt( read[2] );
		
	}
	
	private String mineboardToString( char board[][] ){
		
		String ret = "";
		
		for(int r = 0 ; r < R ; ++ r ){
			for( int c = 0; c < C ; ++ c ){
				ret += board[r][c];
			}
			ret += "\n";
		}
		return ret;
	}
	
	private boolean makeboard( char mineboard[][] ) {
		// board init
		for(int r = 0 ; r < R ; ++ r ){
			for( int c = 0; c < C ; ++ c ){
				mineboard[r][c] = '.';
			}
		}
		
		// make mine board
		
			// put in mine ( row first )
		int direction_rf[][] = new int [4][2];
		direction_rf[0][0] = 1; direction_rf[0][1] = 0;
		direction_rf[1][0] = 0; direction_rf[1][1] = 1;
		direction_rf[2][0] = -1; direction_rf[2][1] = 0;
		direction_rf[3][0] = 0; direction_rf[3][1] = -1;
		
		int direction_cf[][] = new int [4][2];
		direction_cf[0][0] = 0; direction_cf[0][1] = 1;
		direction_cf[1][0] = 1; direction_cf[1][1] = 0;
		direction_cf[2][0] = 0; direction_cf[2][1] = -1;
		direction_cf[3][0] = -1; direction_cf[3][1] = 0;
		
		int direction[][] = direction_cf;
		
		int cur_x = 0, cur_y = 0;
		int now_direction = 0;	// col first  0, row first = 1
		int remind_minecnt = M;
		while( remind_minecnt > 0 ){
			
			mineboard[cur_x][cur_y] = '*';
			remind_minecnt --;
			
			int next_x = cur_x + direction[now_direction][0];
			int next_y = cur_y + direction[now_direction][1];
			
			if( next_x >= R || next_y >= C || next_x < 0 || next_y < 0 ||
				mineboard[next_x][next_y] != '.' ) {
				
				// 방향 전환 
				now_direction ++;
				if( now_direction >= direction.length ) 
					now_direction = 0;
				
				next_x = cur_x + direction[now_direction][0];
				next_y = cur_y + direction[now_direction][1];
				
				if( next_x >= R || next_y >= C || next_x < 0 || next_y < 0 ||
						mineboard[next_x][next_y] != '.' ){
					
					// 방향 전환을 했는대도 자리가 없다면... 이건 Impossible
					return false;
				}
				
			}
			
			cur_x = next_x;
			cur_y = next_y;
			
		}
		
		return true;
	}
	
	
	public boolean putinNumber( char mineboard[][] ){
		// put in number
		
		boolean blankenabled = false;
		int		numbercnt	 = 0;
		int 	lastnumberxy[] 	= new int[2];
		int		firstblank[]	= new int[2];
				
		for( int r = 0; r < R ; r ++ ){
			for( int c = 0 ; c < C ; c ++ ){
				int cnt = 0;
				if( mineboard[r][c] == '.' ){
					for( int i = 0; i < cntbox.length ; ++ i ){
						
						if( ( r + cntbox[i][0] ) >= 0 && ( r + cntbox[i][0] ) < R &&
							( c + cntbox[i][1] ) >= 0 && ( c + cntbox[i][1] ) < C &&
							mineboard[r + cntbox[i][0]][c+cntbox[i][1]] == '*' )
						{
							++ cnt;
						}		
					}
					
					if( cnt > 0 ){
						mineboard[r][c] = ("" + cnt).charAt(0);
						++ numbercnt;
						lastnumberxy[0] = r;
						lastnumberxy[1] = c;
						
					}else{
						if( !blankenabled) {
							firstblank[0] = r;
							firstblank[1] = c;
						}
						blankenabled = true;
					}
				}
			}
		}
		
		if( numbercnt == 0 ){
			mineboard[lastnumberxy[0]][lastnumberxy[1]] = 'c';
		}else if( !blankenabled ){
			return false; 
		}else{ 
			mineboard[firstblank[0]][firstblank[1]] = 'c';
		}
		
		return true;
	}
	
	private void removeNumber( char mineboard[][] ){
		
		// remove number
		for( int r = 0; r < R ; r ++ ){
			for( int c = 0 ; c < C ; c ++ ){
				int cnt = 0;
				if( mineboard[r][c] != '*' &&
					mineboard[r][c] != '.' &&
					mineboard[r][c] != 'c'){
					
					mineboard[r][c] = '.';
				}
			}
		}
		
	}
	
	private boolean isNumber( char ch ){
		return ch >= '1' && ch <= '9';
	}
	

	/**
	 * Number 근처에 blank가 없으면 실패 ~!!
	 * @param mineboard
	 * @return
	 */
	private boolean checkMineboard(char[][] mineboard) {
		
		for( int r = 0; r < R ; r ++ ){
			for( int c = 0 ; c < C ; c ++ ){
				int cnt = 0;
				if( isNumber( mineboard[r][c]) ){
					
					boolean now_fined = false;
					for( int i = 0; i < cntbox.length ; ++ i ){
						
						if( ( r + cntbox[i][0] ) >= 0 && ( r + cntbox[i][0] ) < R &&
							( c + cntbox[i][1] ) >= 0 && ( c + cntbox[i][1] ) < C &&
							( mineboard[r + cntbox[i][0]][c+cntbox[i][1]] == '.' || 
							  mineboard[r + cntbox[i][0]][c+cntbox[i][1]] == 'c' ) )
						{
							now_fined = true;
							break; // check is success
						}		
					}
					if( !now_fined )
						return false; // no blank is impossible
					
				}
			}
		}
		
		return true;
	}
	
	public String check() throws NumberFormatException, IOException{
		
		char mineboard[][] = new char[R][C];
		
		// 보드를 만들고 ~!
		if( !makeboard( mineboard ) ) return "Impossible\n";
		
		
		// 이것은 확인 로직 ~!!
			// 숫자를 넣고, C 를 세긴다.
		if( !putinNumber(mineboard) ) return "Impossible\n";
		
		if( !checkMineboard( mineboard ) ) return "Impossible\n";
		
		// 출력용 로직
		removeNumber( mineboard );
		
		return mineboardToString( mineboard );
		
		
	}


	public static void main(String[] args) throws NumberFormatException, IOException {

		//Read
		File in_file = new File( "./track_c/C-small-attempt0.in" );
		File out_file = new File("./track_c/track_c.out");
		BufferedReader in = new BufferedReader( new FileReader(in_file) );
		BufferedWriter out = new BufferedWriter(new FileWriter( out_file, false ) );

			// read total count
		int totcnt = Integer.parseInt( in.readLine() );
		
		for( int c = 1 ; c <= totcnt ; ++ c ){
			String ret = "Case #" + c + ":\n" + ( new MineSweeperMaster_C( in ).check() ); 
			out.write( ret );
			
			//System.out.print( ret );
		}
		
		
		out.close();
		

	}
}
