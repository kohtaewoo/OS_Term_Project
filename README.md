# OS_Term_Project

## 📌 주제: AWS 환경에서 Linux Shell 구현

### ✅ 추진목표
- C언어 기반으로 Linux Shell 명령어를 직접 구현  
- AWS EC2 인스턴스 환경에서 실습 및 검증  
- 주요 구현 명령어: `ls`, `cd`, `mkdir`, `cat`, `chown`, `clear`, `find`, `rm`  
- 추가 구현 명령어: `tar`, `sl_dongguk`, `pwd`  

### 🎯 연구의 목적
- 운영체제 기본 동작 원리 및 시스템 콜의 이해  
- 멀티스레딩을 활용한 명령어 병렬 처리 구현  
- 클라우드 환경(AWS EC2) 기반의 리눅스 관리 실습 경험  

---

## ⚙️ 프로젝트 구현

### 2.1 Main 함수 및 기타 소스 파일

| 파일명 | 주요 내용 |
|--------|-----------|
| `Makefile` | 빌드, 클린, 재빌드 관리 (`make`, `make clean` 등) |
| `main.c / main.h` | 초기화, 명령어 루프 처리, 사용자 로그인 등 |
| `system.c` | 시간 출력, 프롬프트 표시, 헬퍼 함수 |
| `data_structures.c` | Stack, Deque 자료구조 구현 |
| `filesystem_management.c` | 디렉토리 트리 로딩 및 저장 |
| `permission.c` | 권한 설정 및 검사 함수 |
| `thread.c` | 멀티스레딩 관련 함수 및 분기 처리 |

---

### 2.2 필수 구현 명령어

| 명령어 | 설명 |
|--------|------|
| `ls` | `-a`, `-l`, `-al` 옵션 구현, 색상 구분 출력 |
| `cd` | 상대/절대 경로 이동, 상위 디렉토리(`..`) 이동 |
| `mkdir` | `-p` 옵션, 멀티스레딩 생성 지원 |
| `cat` | `-n`, `-b` 옵션, `>` 리디렉션으로 파일 작성 |
| `chown` | 사용자 변경 및 `user:group` 소유권 변경 |
| `clear` | ANSI 시퀀스로 터미널 클리어 처리 |
| `find` | `-name` 옵션을 통한 트리 기반 파일 검색 |
| `rm` | `-f`, `-r` 옵션으로 강제 삭제 및 재귀 삭제 |

---

### 2.3 추가 구현 명령어

| 명령어 | 설명 |
|--------|------|
| `tar` | `-cvf`, `-xvf` 옵션으로 아카이브 생성 및 해제 (멀티스레딩 지원) |
| `sl_dongguk` | `ls` 오타 시 애니메이션 출력 (`ncurses` 사용) |
| `pwd` | 현재 작업 디렉토리 경로 출력 (`Deque` 사용) |

---

### 2.4 프로그램 실행

#### ✅ 1. 빌드
```bash
make
```

#### ✅ 2. SSH 접속 (팀원)
```bash
ssh -i team4.pem ec2-user@<EC2_PUBLIC_IP>
```

#### ✅ 3. 프로그램 실행
```bash
./shell       # 또는 ./embedded_shell
```

#### ✅ 4. 명령어 테스트 예시
```bash
ls -al
cd ..
mkdir -p dir1/dir2
cat file.txt
cat -n file.txt
tar -cvf archive.tar dir1
tar -xvf archive.tar
find -name test.txt
sl_dongguk
pwd
rm -rf dir1
```

#### ✅ 5. 종료
```bash
exit
```
