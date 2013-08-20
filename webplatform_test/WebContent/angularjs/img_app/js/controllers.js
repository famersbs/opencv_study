
var ImageStorageScope = null;
var sideImageTreeScope = null;

var fileuploadModalScope = null;

var AddImgViewer = function( title ){
	if( null != ImageStorageScope ){
		ImageStorageScope.AddImgViewer( title );
	}
};

function sideImgTreemenu( $scope, $http ){

	$scope.resetImageTree = function(){
		$http.get('../data/filetree.jsp').success( function(data) {
			$scope.treeitem = data.divisions;
			
		});
	};
		
	$scope.itemClick = function( itemname ){
		AddImgViewer( itemname );
	};
	
	// 이미지 트리 리셋
	$scope.resetImageTree();
	
	sideImageTreeScope = $scope;
		
};

function ImageViewCtrl( $scope ){
	
	$scope.mainImage = {
			name:"test1",
			url:"../data/getimage.jsp?name=" + "test1&type="
			};
	
	$scope.imageStore = [];
	
	$scope.AddImgViewer = function( title ){
		$scope.imageStore.push( { name:title, url: "../data/getimage.jsp?name="+title +"&type=" } ); 
	};
	
	$scope.changeMainImg = function(image)
	{
		$scope.mainImage = image;
	};
	
	// 스코프를 전역으로 등록
	ImageStorageScope = $scope;
	
};

function naviGater($scope){
	
	// Modal 값 정의
	//$scope.modal.content = "test content";

} 

function ModalDemoCtrl( $scope ){
	$scope.shouldBeOpen = false;
	
	$scope.open = function () {
		$scope.shouldBeOpen = true;
	};
	
	$scope.close = function () {
		$scope.closeMsg = 'I was closed at: ' + new Date();
		$scope.shouldBeOpen = false;
		
		sideImageTreeScope.resetImageTree();
	};
	
	$scope.items = ['item1', 'item2'];
	
	$scope.opts = {
		backdropFade: true,
		dialogFade:true
	};
		
	fileuploadModalScope = $scope;
}

// 어플리케이션 정의
var myApp = angular.module('myApp', ['$strap.directives']);

//파티셔닝
myApp.filter('partition', function() {
	var part = function(arr, size) {
		if ( 0 === arr.length ) return [];
		return [ arr.slice( 0, size ) ].concat( part( arr.slice( size ), size) );
	};
	return part;
});


//sideImgTreemenu.$inject = ['$scope', '$http'];
