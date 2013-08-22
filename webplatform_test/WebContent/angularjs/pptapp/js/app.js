var app = angular.module('pptApp', []);


app.config(['$routeProvider', function($routeProvider) {
	  
	  /*
  $routeProvider.
      when('/phones', {templateUrl: 'phone-list.html',   controller: PhoneListCtrl}).
      when('/phones/:phoneId', {templateUrl: 'phone-detail.html', controller: PhoneDetailCtrl}).
      otherwise({redirectTo: '/phones'});
      */
}]);


// 동적 코드

app.directive('pptapp', function($compile) {

	return{
		restrict:'E',
		replace:true,
		scope: {              // set up directive's isolated scope
		      name: "@",          // name var passed by value (string, one-way)
		      amount: "=",        // amount var passed by reference (two-way)
		      save: "&"           // save action
		    },
	    template:             // replacement HTML (can use our scope vars here)
	        "<div id=\"myCarousel\" class=\"carousel slide\" data-interval=\"false\" ng-click=\"onClickPage()\">" +
		      "<div class=\"carousel-inner\">" +
		        "<div ng-class=\"{active: $first}\" class=\"item\" ng-repeat=\"page in pptbook\">" +
		          "<div class=\"container\" >" +
		            "<div class=\"carousel-caption\">" +
		              "<ng-include src=\"page.url\"></ng-include>" +
		              "<div class=\"ppt_footer\">{{$index + 1}} / {{pptbook.length}}</div>" +
		            "</div>" +
		          "</div>" +
		        "</div>" +
		      "</div>" +
		      "<a class=\"left carousel-control\" href=\"#myCarousel\" data-slide=\"prev\">&lsaquo;</a>" +
		      "<a class=\"right carousel-control\" href=\"#myCarousel\" data-slide=\"next\">&rsaquo;</a>" +
		    "</div>",
        replace: true,        // replace original markup with template
        transclude: false,    // do not copy original HTML content
        controller: [ "$scope", "$http", function ($scope, $http) {
        	$scope.pptbook = [];        	
        	
        	$scope.load = function( url ){
	        	$http.get( url ).success( function(data) {
	        		$scope.pptbook = data;
	        		
	        	});
        	}
        	
        	$scope.onClickPage = function(){
        		$("#myCarousel").find('.active').next();
        	};
        }],
		link: function( scope, ele, attrs, controller ){
			// url 을 이요해서 controller 를 작동 시킵니다.
			scope.load( attrs.url );
		}
	};
	
});