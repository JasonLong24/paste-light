pipeline {
  agent {
    docker { image 'gcc:8.3' }
  }
  stages {
    stage('Build') {
      steps {
        sh 'gcc --version'
        sh 'make clean && make -j'
      }
    }
    stage('Test') {
      steps {
        sh 'make tests'
        sh 'cd tests && ./tests/test_paste_light'
      }
    }
  }
}
