
function pptPageCtrl( $scope, $http ){
	
	$scope.pptbook = [];
	
	
	$http.get('pptapp/ppt_book/page.json').success( function(data) {
		$scope.pptbook = data;
		
	});
	
	$scope.onClickPage = function(){
		console.log( $("#myCarousel") );
		
		$("#myCarousel").find('.active').next();
		
		//$(".right.carousel-control").click();
	};

}

// pptPageCtrl.$inject = ['$scope', '$http'];